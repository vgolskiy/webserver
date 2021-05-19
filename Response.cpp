#include "Response.hpp"

Response::Response(Client *client, t_server *server, std::string loc, std::string requested_index) : _client(client), _requested_index(requested_index) {
	_response = "";
	_method = client->get_request()->get_method();
	_body = "";
	_loc = get_location(server, loc);
	_status = client->get_request()->get_status();
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

void Response::fill_response_body(void) {
	std::map<std::string, std::string>::iterator it;
	for (it = _headers.begin(); it != _headers.end(); ++it) {
		if ((*it).second != "")
			_response += (*it).first + ": " + (*it).second + CRLF;
	}
	return;
}

std::string	Response::get_page_body(void) {
	std::string		res = "";
	std::string		file;
	std::stringstream ss;

	file = _requested_index.length() ? _loc->root + _requested_index : _loc->root + _loc->index[0];

	std::ifstream	inf(file);

	if (!inf){
		throw std::runtime_error(file);
	}
	ss << inf.rdbuf();
	res += ss.str();
	res += CRLF_2X;
	inf.clear();
	return (res);
}

void Response::create_response(void) {
	//_response = "";
	//_headers["Allow"] =
	//_headers["Location"] =
	_headers["Retry-After"] = "1";
	_headers["Server"] = "webserv";
	_headers["Date"] = get_server_date();
	//_headers["WWW-Authenticate"] =
	//_headers["Last-Modified"] = get_last_modified_date();
	if (_method == "HEAD")
	{
		_response += HTTP;
		//_response += _status.
		_response += CRLF;
		fill_response_body();
		_response += _body + CRLF;
	}
	else if (_method == "GET") {
		_response += "HTTP/1.1 200 OK\r\n";
		if (_client->get_request()->get_uri() == CWN || _client->get_request()->get_uri() == RMN 
			|| _client->get_request()->get_uri() == MSK || _client->get_request()->get_uri() == HHP2
			|| _client->get_request()->get_uri() == HHP)
			_response += "Date: Sun, 18 Oct 2012 10:36:20 GMT\r\nServer: Webserver\r\nContent-Type: image/*\r\n\r\n";
		else
			fill_response_body();
		_response += get_page_body();
	}
	else if (_method == "PUT" || _method == "POST")
	{
		_response += _client->get_request()->get_body();
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
	}
	else if (_method == "POST")
	{
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

