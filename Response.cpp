#include "Response.hpp"

Response::Response(Client *client, t_server *server, std::string loc, std::string requested_index) : _client(client), _requested_index(requested_index) {
	_status_code = client->get_request()->get_status_code() ? client->get_request()->get_status_code() : 200;
	set_status();
	_response = "";
	_method = client->get_request()->get_method();
	_body = "";
	_loc = get_location(server, loc);
	_param = client->get_request()->get_uri_parameters();
	_authorize = client->get_request()->get_authorization();
	_error_page = server->error_page;

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
		throw std::runtime_error(file);
	}
	ss << inf.rdbuf();
	res += ss.str();
	res += CRLF_2X;
	inf.close();
	return (res);
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

	if (_client->get_request()->get_status() == Request::BAD_REQ) // Bad_Req - for everything not defined
	{
		_status_code = 400;
		get_status_line();
		fill_response_body();
		_response += CRLF;
		//send html => _response += get_page_body() + CRLF;
	}

	//set status according to the request's method:
	// GET - if Request::BADREQ -> _status_code = NOT_FOUND
	// GET - else if loc.auth != empty ->

	//_headers["Allow"] =
	//_headers["Location"] =
	_headers["Retry-After"] = "1";
	_headers["Server"] = "webserv";
	_headers["Date"] = get_server_date();
	_headers["Content-Type"] = get_content_type();
	//_headers["WWW-Authenticate"] =
	//_headers["Last-Modified"] = get_last_modified_date();
	if (_method == "HEAD") {
		//Status code always OK beac
		_status_code = 200;
		get_status_line();
		fill_response_body();
	}
	else if (_method == "GET") {
		// if (_client->get_request()->get_status() == UNATHOURIZED) // depends on when you check
			// _status_code = 401
		// else if (_client->get_request()->get_status() == Not_found)
			// _status_code = 404;
		// else (_client->get_request()->get_status() == AUTHORIZED)
			// check_authorization
			// if (not valid authorization)
				// _status_code = 401;
				// fill in _headers[WWW-Authenticate] appropriately
		if (_loc->auth.size()) {
			if ((!auth_by_header()) && (!auth_by_uri_param()))
				_status_code = 401;
		}
		get_status_line();
		fill_response_body();
		_response += get_page_body() + CRLF;
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
		// check location
	}
}

void Response::set_status()
{
	_status[200] = "OK"; //get head post
	_status[201] = "Created"; //put
	_status[202] = "Accepted";
	_status[204] = "No Content"; //нет содержимого для put
	_status[301] = "Moved Permanently"; //если uri изменен
	_status[302] = "Found"; //get или head + в ответе будет новый локейшн
	_status[304] = "Not Modified"; //используется для кеширования
	_status[400] =  "Bad Request";
	_status[401] =  "Unauthorized"; //нужна аутентификация
	_status[403] =  "Forbidden";
	_status[404] =  "Not Found";
	_status[405] =  "Method Not Allowed"; //не возвращают этот код head и get
	_status[500] =  "Internal Server Error"; //внутренняя ошибка сервера
	_status[501] =  "Not Implemented"; //не возвращают этот код head и get
	_status[502] =  "Bad Gateway";
	_status[503] =  "Service Unavailable"; //отключен или перегружен, не готов обрабатывать запрос
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
