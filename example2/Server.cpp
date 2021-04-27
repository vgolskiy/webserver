//
// Created by Kanat Saribaew on 3/5/21.
//

#include "Server.hpp"
#include "cgi/Request.hpp"

Server::Server(const t_server_config_data &config) :  _generate_headers(config.mime_map),  _gen_page(config.error_page), _config(config), _client(){
	_socket.setAddres(config.ip);
	_socket.setPort(config.port);
	try {
		_socket.listen_socket();
		_socket_server = _socket.getSock();
	}catch(std::exception &except) {
		std::cerr << except.what()<<std::endl;
		exit(1);
	}
}
std::string Server::_get_remote_addr(int fd, sockaddr_in *addr, socklen_t *len) {
	getsockname(fd, reinterpret_cast<sockaddr *>(addr), len);
	uint32_t ip = addr->sin_addr.s_addr;
	int octet1 = static_cast<int>((ip >> 24) & 0xFF);
	int octet2 = static_cast<int>((ip >> 16) & 0xFF);
	int octet3 = static_cast<int>((ip >> 8) & 0xFF);
	int octet4 = static_cast<int>(ip & 0xFF);
	std::stringstream stream;
	stream << octet4 << "." << octet3 << "." << octet2 << "." << octet1;
	return std::string(stream.str());
}

int Server::getSocketServer() const {
	return _socket_server;
}

const std::vector<Client> &Server::getClient() const {
	return _client;
}
std::vector<std::string> Server::_get_method(Http &http) {
	std::vector<std::string>response(2, "");
	std::string body;
	std::string line;
	std::string header;
	std::string target_file = http.getStartLine().find("change_location")->second;
	target_file += http.getStartLine().find("source")->second;
	std::ifstream in(target_file);
	if (in.is_open()){
		while (std::getline(in, line)){
			body += line;
			if (!in.eof()){
				body += "\n";
			}
		}
		header = _generate_headers.get_status("200");
		header += _generate_headers.get_server();
		header += _generate_headers.get_server_date();
		header += _generate_headers.get_content_type(_get_extensions(http.getStartLine().find("source")->second));
		header += _generate_headers.get_content_length(std::to_string(body.length()));
		header += _generate_headers.get_last_modified(target_file);
		header += "\r\n";
		response[0] = header;
		response[1] = body;
	}
	return response;
}
std::vector<std::string> Server::_head_method(Http &http) {
	std::vector<std::string> response = _get_method(http);
	response[1].clear();
	return response;
}
std::vector<std::string> Server::_put_method(Http &http, t_locations &locations) {
	std::string path = http.getStartLine().find("change_location")->second + http.getStartLine().find("source")->second;
	std::string tmp_path = locations.upload_storage + http.getStartLine().find("source")->second;
	std::string tmp_dir = locations.upload_storage;
	std::string headers;
	std::vector<std::string>response(2, "");
	struct stat	buf;
	int			result;
	int 		fd;

	result = stat(path.c_str(), &buf);
	if (result == 0){
		fd = open(path.c_str(), O_CREAT | O_RDWR | O_APPEND, 0666);
		headers = _generate_headers.get_status("200");
	}else{
		fd = open(path.c_str(), O_CREAT | O_RDWR | O_TRUNC, 0666);
		headers = _generate_headers.get_status("201");

	}
	write(fd, http.getBody().c_str(), http.getBody().length());
	result = stat(tmp_dir.c_str(), &buf);
	if (result == 0) {
		result = stat(tmp_path.c_str(), &buf);
		if (result == 0) {
			fd = open(tmp_path.c_str(), O_CREAT | O_RDWR | O_APPEND, 0666);
		} else {
			fd = open(tmp_path.c_str(), O_CREAT | O_RDWR | O_TRUNC, 0666);
		}
		write(fd, http.getBody().c_str(), http.getBody().length());
	}
	headers += _generate_headers.get_server();
	headers += _generate_headers.get_server_date();
	headers += _generate_headers.get_content_length("0");
	headers += "Content-Location: " + http.getStartLine().find("location")->second + http.getStartLine().find("source")->second + "\r\n\r\n";
	response[0] = headers;
	response[1] = "";
	return response;
}
void Server::_controller(Client &client, t_locations &locations, std::string &method, std::pair<bool, size_t>p)
{
	Http http = client.getHttp();
	if (p.first && method != "PUT"){
		std::vector<std::string>response(2, "");
		std::string header;
		std::vector<std::string> ret = requestBody(client, _config, locations, method, locations.cgi_path[p.second]);
		header = ret[1];
        header.insert(0, _generate_headers.get_status(ret[0]));
        header += _generate_headers.get_server();
        header += _generate_headers.get_server_date();
        header += _generate_headers.get_content_length(std::to_string(ret[2].length()));
        header += "\r\n";
        response[0] = header;
        response[1] = ret[2];
		http.setResponse(response);
		http.setStatus("write");
	}else{
		http.setStatus("write");
		if (method == "HEAD"){
			http.setResponse(_head_method(http));
		}else if (method == "GET"){
			http.setResponse(_get_method(http));
		}else
			http.setResponse(_put_method(http, locations));
	}
	client.setHttp(http);
}

void Server::new_connection() {
	sockaddr_in addr;
	socklen_t size = sizeof(sockaddr_in);
	try {
		 int sock =_socket.accept_socket(&addr, &size);
		 std::string address = _get_remote_addr(sock, &addr, &size);
		_client.push_back(Client(sock, address, _config.all_method, _config));
	}catch (std::exception &except){
		std::cerr << except.what()<<std::endl;
	}
}

void Server::recive(int index_client) {
	Http http = _client[index_client].getHttp();
	RequestParser parser = _client[index_client].getParserRequest();
	if (_socket.receive(_client[index_client].getFd(), http)){
		parser.parser(http);
		_client[index_client].setHttp(http);
		if (http.getStatus() == "error" || http.getStatus() == "execute"){
			_execute_methods(_client[index_client]);
		}
	}else{
		close_connect(index_client);
	}
}
void Server::response(int index_client) {
	Http http = _client[index_client].getHttp();
	std::vector<std::string> response = http.getResponse();
	std::string status;

	size_t length = 1000000;
	size_t index = 0;
	if (response[0].length()){
		index = 0;
		length = response[0].length() < length ? response[0].length() : length;
	}else if (response[1].length()){
		index = 1;
		length = response[1].length() < length ? response[1].length() : length;
	}
	length = _socket.response(_client[index_client].getFd(), response[index]
						   , length);
	response[index].erase(0, length);
	http.setResponse(response);
	if (response[0].empty() && response[1].empty()) {
		std::map<std::string, std::string>::const_iterator it;
		if ((it = http.getHeadMap().find("connection")) != http.getHeadMap().end() && it->second == "close"){
			close_connect(index_client);
			std::cout << "s"<<std::endl;
			return;
		}
		http.setStatus("read_header");
		http.clear();
		http.i = 0;
	}
	_client[index_client].setHttp(http);
}


void Server::_execute_methods(Client &client) {
	Http http = client.getHttp();
	std::pair<bool, std::pair<size_t, size_t> >p = std::make_pair(false, std::make_pair(http.getErrorFlag(), 0));
	if (http.getStatus() == "error"){
		std::cerr<<""<<std::endl;
	}
	if (http.getStatus() != "error") {
		http.setStatus("write");
		if ((p = _checking_сorrectness_of_request(http)).first){
			client.setHttp(http);
			t_locations loc = _config.location[p.second.first];
			std::string method = loc.method[p.second.second];
			_controller(client, loc, method, who_execute(loc,  http.getStartLine().find("source")->second));
			return;
		}
		std::string redirect = http.getStartLine().find("uri")->second;
		_error_handler(http, p.second.second, _config.location[p.second.first],  redirect);
	}else
		_error_handler(http, http.getErrorFlag(), _config.location[0],  std::string(""));
	client.setHttp(http);
}

std::pair<bool, size_t> Server::_check_locations(const std::string &loc, const std::vector<t_locations> &locs) {
	std::vector<t_locations>locations = locs;
	std::string location = loc;
	for (size_t i = 0; i < locations.size(); ++i){
		if (location.find(locations[i].location) != std::string::npos){
			return std::pair<bool, int>(true, i);
		}
	}
	return std::pair<bool, int>(false, 404);
}
std::pair<bool, size_t> Server::_check_methods(const std::string &method, const std::vector<std::string> &methods){
	for (size_t i = 0; i < methods.size(); ++i) {
		if (methods[i] == method){
			return std::pair<bool, size_t>(true, i);
		}
	}
	return std::pair<bool, size_t>(false, 405);
}
std::pair<bool, std::string> Server::_check_source(const std::string &location, const std::string &source, t_locations &loc){
	struct stat	buf;
	int			result;
	DIR *dir;
	struct dirent	*entry;
	std::string full_path = location +source;
	std::vector<std::string>s;
	std::vector<std::string>index_types = loc.index_types;
	if (who_execute(loc, source).first){
		return std::pair<bool, std::string>(true, std::string(source));
	}
	if (source.empty() && (dir = opendir(location.c_str()))){
		while (dir && (entry = readdir(dir)) != nullptr) {
			s.push_back(entry->d_name);
		}
		closedir(dir);
		for (size_t  i = 0; i < index_types.size(); ++i) {
			for (size_t j = 0; j < s.size(); ++j) {
				if (s[j] == index_types[i]){
					return std::pair<bool, std::string>(true, std::string(s[j]));
				}
			}
		}
		return std::pair<bool, std::string>(false, std::string("1000"));
	}
	result = stat(full_path.c_str(), &buf);
	if (result == 0){
		if (!S_ISREG(buf.st_mode)){
			return std::pair<bool, std::string>(false, std::string("301"));
		}else{
			return std::pair<bool, std::string>(true, std::string(source));
		}
	}
	return std::pair<bool, std::string>(false, std::string("404"));
}
void Server::_error_handler(Http &http, int cod_error, t_locations &loc, std::string l) {
	std::string body;
	std::string header;
	std::string special_header;
	std::vector<std::string>response(2, "");
	http.setStatus("write");
	http.setErrorFlag(cod_error);
	header = _generate_headers.get_server();
	header += _generate_headers.get_server_date();
	header += _generate_headers.get_content_type(".html");
	if (cod_error == 1000 && loc.autoindex){
		body = _gen_page.gen_auto_page(http.getStartLine().find("change_location")->second, http.getStartLine().find("location")->second);
		header.insert(0, _generate_headers.get_status("200"));
		header += _generate_headers.get_content_length(std::to_string(body.length()));

	}else{
		cod_error = cod_error == 1000 ? 404 : cod_error;
		body = _gen_page.gen_error_page(cod_error);
		header.insert(0, _generate_headers.get_status(std::to_string(cod_error)));
		header += _generate_headers.get_content_length(std::to_string(body.length()));
	}
	if (!(special_header = _generate_headers.generate_special_headers(cod_error, loc, l)).empty()){
		header += special_header;
	}
	header += "\r\n";
	response[0] = header;
	if (http.getStartLine().find("method")->second != "HEAD")
		response[1] = body;
	http.setResponse(response);
}

std::pair<bool, std::pair<size_t, size_t> > Server::_checking_сorrectness_of_request(Http &http) {
	std::string location =http.getStartLine().find("location")->second;
	std::string location2 = http.getStartLine().find("source")->second + "/";
	std::map<std::string, std::string>m_start_line = http.getStartLine();


	location.insert(0, _config.root);
	location2 = location + location2;
	std::pair<bool, size_t> p_method(false, 0);
	std::pair<bool, std::string> p_source(false, "");
	std::pair<bool, size_t> p_loc = _check_locations(location, _config.location);
	std::pair<bool, size_t> p_loc2 = _check_locations(location2, _config.location);
	std::pair<bool, size_t> p_auth(false, 0);
	if (!p_loc.first) {
		return std::make_pair(false, std::make_pair(0, p_loc.second));
	}
	if (p_loc2.first && p_loc.second != p_loc2.second){

		p_loc = p_loc2;
		m_start_line["source"] = "";
//		size_t pos = location2.find( _config.location[p_loc2.second].location);
//		int status = m_start_line.find("method")->second != "GET" &&  m_start_line.find("method")->second != "HEAD" ? 308 : 301;
//		location2.erase(0, pos + _config.location[p_loc2.second].location.length());
//		m_start_line["change_location"] = _config.location[p_loc2.second].root + location2;
//		http.setStartLine(m_start_line);
//		return std::make_pair(false, std::make_pair(p_loc2.second,status));
	}
	if (!(p_auth = _check_authorization(http, _config.location[p_loc.second])).first){
		return std::make_pair(false, std::make_pair(p_loc.second, p_auth.second));
	}
	if (!(p_method = _check_methods(http.getStartLine().find("method")->second, _config.location[p_loc.second].method)).first) {
		if (p_method.second == 501){
			p_method.second = 501;
		}
		return std::make_pair(false, std::make_pair(p_loc.second, p_method.second));
	}
	size_t pos = location.find( _config.location[p_loc.second].location);
	location.erase(0, pos + _config.location[p_loc.second].location.length());
	m_start_line["change_location"] = _config.location[p_loc.second].root + location;
	if (_config.location[p_loc.second].method[p_method.second] == "PUT"){
		m_start_line["path_info"] = m_start_line["change_location"] + m_start_line["source"];
		http.setStartLine(m_start_line);
		return std::make_pair(true, std::make_pair(p_loc.second, p_method.second));
	}
	if (_config.location[p_loc.second].max_body_size < http.getBody().length()){
		http.setStartLine(m_start_line);
		return std::make_pair(false, std::make_pair(p_loc.second, 413));
	}
	if ((p_source = _check_source(m_start_line["change_location"], m_start_line["source"], _config.location[p_loc.second])).first) {
		m_start_line["source"] = p_source.second;
		m_start_line["path_info"] = m_start_line["change_location"] + m_start_line["source"];
		http.setStartLine(m_start_line);
		return std::make_pair(true, std::make_pair(p_loc.second, p_method.second));
	}else{
		http.setStartLine(m_start_line);
		return std::make_pair(false, std::make_pair(p_loc.second, atoi(p_source.second.c_str())));
	}
}

std::pair<bool, size_t> Server::who_execute(t_locations &loc, std::string execute) {
	size_t pos;
	std::vector<std::string>extensions = loc.cgi_extensions;
	for (size_t  i = 0; i < extensions.size(); ++i) {
		if ((pos = execute.find(extensions[i])) != std::string::npos && execute.substr(pos) == extensions[i] && execute.length() != extensions[i].length()){
			return std::make_pair(true, i);
		}
	}
	return std::make_pair(false, 0);;
}

std::string Server::_get_extensions(const std::string &str) {
	for (size_t i = str.length(); i > 0; --i) {
		if (str[i] == '.'){
			return str.substr(i);
		}
	}
	return std::string();
}

std::pair<bool, size_t> Server::_check_authorization(Http &http, t_locations &loc) {
	std::map<std::string, std::string>::const_iterator it;
	if (!loc.auth)
		return std::make_pair(true, 0);
	if ((it = http.getHeadMap().find("authorization")) == http.getHeadMap().end())
		return  std::make_pair(false, 401);
	else{
		std::string auth = it->second;
		size_t pos = auth.find_first_of(' ');
		std::string auth_type = auth.substr(0, pos);
		auth.erase(0, pos + 1);
		pos =  auth.find_first_of(':');
		if (pos == std::string::npos){
			if (auth_type != loc.auth_data.AuthType || base64_decode(auth) != loc.auth_data.login + ":" + loc.auth_data.password){
				return std::make_pair(false, 401);
			}
		}else {
			std::string auth_user = auth_type.substr(0, pos);
			auth.erase(0, pos + 1);
			std::string auth_pass = auth;
			if (auth_type != loc.auth_data.AuthType || auth_user != loc.auth_data.login ||
				base64_decode(auth_pass) != loc.auth_data.password) {
				return std::make_pair(false, 401);
			}
		}
	}
	return std::make_pair(true, 0);
}

void Server::close_connect(int index_client) {
	close(_client[index_client].getFd());
	_client.erase(_client.begin() + index_client);
}

