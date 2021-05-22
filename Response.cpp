#include "Response.hpp"

#define HTML_TITLE "<html>\n<head>\n<title>Listing of directories</title>\n</head>"
#define HTML_HEADER "<body>\n<h1>Autoindex: </h1>\n"
#define HYPER_REF "<a href=\""
#define HYPER_END "</a><br>"
#define HTML_CLOSE "</body>\n</html>\n"

Response::Response(Client *client, t_server *server, std::string loc, std::string requested_index) : _client(client), _requested_index(requested_index) {
	_status_code = client->get_request()->get_status_code() ?
		client->get_request()->get_status_code() : 200;
	set_status();
	_response = "";
	_method = client->get_request()->get_method();
	_body = "";
	_loc = get_location(server, loc);
	_param = client->get_request()->get_uri_parameters();
	_authorize = client->get_request()->get_authorization();
	_error_page = server->error_page;
	_content_len = client->get_request()->get_content_length();
	
	std::vector<std::string> v;
	const char* ss[4] = {".gif", ".jpeg", ".jpg", ".png"};
	for (int i = 0; i < 4; ++i)
		v.push_back(ss[i]);
	_content_types["image/gif"] = v;
	v.clear();
	v.push_back(".mp4");
	_content_types["video/mpeg"] = v;
	v.clear();
	v.push_back(".woff");
	_content_types["font/woff"] = v;
	v.clear();
	v.push_back(".woff2");
	_content_types["font/woff2"] = v;

	std::string tmp[] = {
		"Allow",
		"Content-Language",
		"Content-Length",
		"Content-Location",
		"Content-Type",
		"Host",
		"Last-Modified",
		"Server",
		"WWW-Authenticate"};
	for (int i = 0; i < 9; ++i)
		_headers_sequence.push_back(tmp[i]);
}

Response::~Response() {}

std::string Response::get_response_body(void) { return (_response); }

std::string Response::get_server_date(void)	{
	struct tm		info;
	struct timeval	time;
	char			buf[29];
	std::string		s;

	gettimeofday(&time, NULL);
	s = std::to_string(time.tv_sec);
	strptime(s.c_str(), " %s ", &info);
	strftime(buf, sizeof(buf), "%a, %d %b %Y %X", &info);
	s = buf;
	s += " GMT";
	return (s);
}

std::string Response::get_last_modified_date(std::string file) {
	struct tm	info;
	char		buf[29];
	struct stat	st;
	std::string	s;

	stat(file.c_str(), &st);
	s = std::to_string(st.st_mtimespec.tv_sec);
	strptime(s.c_str(), " %s ", &info);
	strftime (buf, sizeof(buf), "%a, %d %b %Y %X", &info);
	s = buf;
	s += " GMT";
	return (s);
}

void Response::get_status_line(void) {
	_response = HTTP + std::string(" ") + std::to_string(_status_code)
		+ std::string(" ") + _status[_status_code];
	_response += CRLF;
}

void Response::fill_response_body(void) {
	std::list<std::string>::iterator it;

	for (it = _headers_sequence.begin(); it != _headers_sequence.end(); ++it) {
		if (_headers[*it] != "")
			_response += *it + ": " + _headers[*it] + CRLF;
	}
	_response += CRLF;
	return ;
}

std::string	Response::get_page_body(void) {
	std::string		res = "";
	std::string		file;
	std::stringstream ss;

	if (_status_code == 200)
		file = _requested_index.length() ? _loc->root + _requested_index : _loc->root + _loc->index[0];
	else
		file = _error_page[std::to_string(_status_code)];

	std::ifstream	inf(file);

	if (!inf){
		return res;
	}
	ss << inf.rdbuf();
	res += ss.str();
	res += CRLF_2X;
	inf.close();
	return (res);
}

void	Response::create_autoindex(){
	//root - is a full path for a current location
	DIR *directory = opendir(_loc->root.c_str());
	if (!directory)
		return ;
	
	struct dirent *curr;

	_body += HTML_TITLE;
	_body += HTML_HEADER;
	while ((curr = readdir(directory))) {
		if (curr->d_name[0] != '.') {
			_body += HYPER_REF;
			_body += curr->d_name;
			_body += "\">";
			_body += curr->d_name;
			_body += HYPER_END;
		}
	}
	_body += HTML_CLOSE;
	closedir(directory);
}

std::string	Response::get_content_type() {
	std::string	tmp;
	if (_requested_index.length()) {
		tmp = _requested_index.substr(_requested_index.rfind("."));

		std::map<std::string, std::vector<std::string> >::iterator	itm;
		for (itm = _content_types.begin(); itm != _content_types.end(); ++itm) {
			std::vector<std::string>::iterator	itv;

			for (itv = (*itm).second.begin(); itv != (*itm).second.end(); ++itv) {
				if (tmp == (*itv))
					return ((*itm).first);
			}
		}
	}
	return ("text/html");
}

//Verification of opened credentials vs opened user and encrypted password in location
bool Response::auth_by_uri_param(void) {
	std::map<std::string, std::string>::iterator	it;
	std::string					usr, psw;

	if (_param.length()) {
		if (_param.find("user=") != std::string::npos) {
			usr = _param.substr(_param.find("user=") + 5);
			psw = usr.substr(usr.find("&") + 1);
			if (!psw.find("password=")) {
				usr = usr.substr(0, usr.find("&"));
				psw = split(psw.substr(9, std::string::npos), "&")[0];
				psw = base64_encode(psw);
				if (((it = _loc->auth.find(usr)) != _loc->auth.end())
					&& ((*it).second == psw))
					return (true);
			}
		}
	}
	return (false);
}

//Verification of closed credentials vs opened user and encrypted password in location
bool Response::auth_by_header(void) {
	std::map<std::string, std::string>::iterator	it;
	std::vector<std::string>	pair;
	std::string					tmp;

	if (_authorize.length()) {
		tmp = base64_decode(_authorize);
		pair = split(tmp, ":");
		if (((it = _loc->auth.find(pair[0])) != _loc->auth.end())
					&& ((*it).second == base64_encode(pair[1])))
			return (true);
	}
    return (false);
}

void Response::create_response(void) {
	std::string tmp;

	//_headers["Allow"] =
	//_headers["Location"] =
	_headers["Retry-After"] = "1";
	_headers["Server"] = "webserv";
	_headers["Date"] = get_server_date();
	_headers["Content-Type"] = get_content_type();
	//if (_content_len)
	//	_headers["Content-Length"] = std::to_string(_content_len);
	//_headers["WWW-Authenticate"] =
	//_headers["Last-Modified"] = get_last_modified_date();
	if (_client->get_request()->get_status() == Request::BAD_REQ) // Bad_Req - for everything not defined
	{
		if (_client->get_request()->get_status_code() == 0)
			_status_code = 400;
		get_status_line();
		fill_response_body();
		_response += get_page_body();
		_response += CRLF;
	}
	else if (_method == "HEAD") {
		get_status_line();
		fill_response_body();
	}
	else if (_method == "GET") {
		if (_loc->auth.size()) {
			if ((!auth_by_header()) && (!auth_by_uri_param()))
				_status_code = 401;
		}
		else if (_status_code != 401 && _loc->auto_index)
			create_autoindex();
		get_status_line();
		fill_response_body();
		_response += _body;
		if (!_loc->auto_index)
			_response += get_page_body();
	}
	else if (_method == "PUT")
	{
		// know the file to change
		// check everything about file (lstat, opendir, etc)
		// ...
		_response += _client->get_request()->get_body();
	}
	else if (_method == "POST")
	{
		_client->get_request()->parse_script_file_name();
		_client->get_request()->set_cgi_meta_vars();
		if (_client->get_request()->get_script_name())
		{
			_client->get_request()->run_cgi_request(); // TODO: add if-condition in case of error
			_client->get_request()->read_cgi();
			get_status_line();
			fill_response_body();
			_response += _client->get_request()->get_body();
		}
		else
		{
			// make the route able to accept uploaded files and configure where it should be saved (subject)
			if (_status_code != 401 && _loc->auto_index)
				create_autoindex();
			get_status_line();
			fill_response_body();
			_response += _client->get_request()->get_body();
		}
	}
	else
	{
		_status_code = 1; // not implemented;
		get_status_line();
		fill_response_body();
		_response += _client->get_request()->get_body(); // ?
	}
}

void Response::set_status()
{
	_status[1] = "Not Implemented";
	_status[100] = "Continue";
	_status[101] = "Switching Protocols";
	_status[102] = "Processing";
	_status[200] = "OK";
	_status[201] = "Created";
	_status[202] = "Accepted";
	_status[203] = "Non-authoritative Information";
	_status[204] = "No Content";
	_status[205] = "Reset Content";
	_status[206] = "Partial Content";
	_status[207] = "Multi-Status";
	_status[208] = "Already Reported";
	_status[226] = "IM Used";
	_status[300] = "Multiple Choices";
	_status[301] = "Moved Permanently";
	_status[302] = "Found";
	_status[303] = "See Other";
	_status[304] = "Not Modified";
	_status[305] = "Use Proxy";
	_status[307] = "Temporary Redirect";
	_status[308] = "Permanent Redirect";
	_status[400] =  "Bad Request";
	_status[401] =  "Unauthorized";
	_status[402] =  "Payment Required";
	_status[403] =  "Forbidden";
	_status[404] =  "Not Found";
	_status[405] =  "Method Not Allowed";
	_status[406] =  "Not Acceptable";
	_status[407] =  "Proxy Authentication Required";
	_status[408] =  "Request Timeout";
	_status[409] =  "Conflict";
	_status[410] =  "Gone";
	_status[411] =  "Length Required";
	_status[412] =  "Precondition Failed";
	_status[413] =  "Payload Too Large";
	_status[414] =  "Request-URI Too Long";
	_status[415] =  "Unsupported Media Type";
	_status[416] =  "Requested Range Not Satisfiable";
	_status[417] =  "Expectation Failed";
	_status[418] =  "I'm a teapot";
	_status[421] =  "Misdirected Request";
	_status[422] =  "Unprocessable Entity";
	_status[423] =  "Locked";
	_status[424] =  "Failed Dependency";
	_status[426] =  "Upgrade Required";
	_status[428] =  "Precondition Required";
	_status[429] =  "Too Many Requests";
	_status[431] =  "Request Header Fields Too Large";
	_status[444] =  "Connection Closed Without Response";
	_status[451] =  "Unavailable For Legal Reasons";
	_status[499] =  "Client Closed Request";
	_status[500] =  "Internal Server Error";
	_status[501] =  "Not Implemented";
	_status[502] =  "Bad Gateway";
	_status[503] =  "Service Unavailable";
	_status[504] =  "Gateway Timeout";
	_status[505] =  "HTTP Version Not Supported";
	_status[506] =  "Variant Also Negotiates";
	_status[507] =  "Insufficient Storage";
	_status[508] =  "Loop Detected";
	_status[510] =  "Not Extended";
	_status[518] =  "I am a teapot";
	_status[511] =  "Network Authentication Required";
	_status[599] =  "Network Connect Timeout Error";
}

		//PUT /new.html HTTP/1.1
		//Host: example.com
		//Content-type: text/html
		//Content-length: 16
		//
		//<p>Новый файл</p>
		//создать
		//HTTP/1.1 201 Created
		//Content-Location: /new.html
		//обновить
		//HTTP/1.1 204 No Content //или 200 OK
		//Content-Location: /existing.html
		// PUT /user/1234567890 HTTP/1.1
		//Host: localhost
		// {
		//	"name": "Kevin Sookocheff",
		//	"website": "http://kevinsookocheff.com"
		//}
		//ответы:
		//HTTP/1.1 201 Created 200 OK 204 No Content
		//Location: /user/1234567890
		// или например заменить
		//PUT /user/1234567890 HTTP/1.1
		//Host: http://sookocheff.com
		//
		//{
		//	"name": "Kevin Sookocheff",
		//	"website": "http://sookocheff.com"
		//}
