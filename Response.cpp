#include "Response.hpp"

Response::Response(Client *client) : _client(client) {
	_response = "";
	_method = "";
	_body = "";
}

Response::~Response() {}

std::string Response::get_response_body(void) { return (_response); }

std::string Response::server_date(void)	{
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

std::string Response::last_modified(std::string file) {
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
}

void Response::create_response(void) {
	_response = "";
	//_headers["Allow"] =
	//_headers["Location"] =
	_headers["Retry-After"] = 1;
	_headers["Server"] = "webserv";
	_headers["Date"] = server_date();
	//_headers["WWW-Authenticate"] =
	//_headers["Last-Modified"] = last_modified();
	if (_method == "HEAD")
	{
		_response += HTTP;
		//_response += _status.
		_response += CRLF;
		fill_response_body();
		_response += _body + CRLF;
	}
	else if (_method == "GET") {
		fill_response_body();
		_response += _client->get_request()->get_body() + CRLF;
		//std::string path = _location; //
		//DIR *dir;
		//struct dirent *d;
		//if (dir = opendir(path.c_str()))
		//{
			//_response += ;
		//}
	}
	else if (_method == "PUT" || _method == "POST")
	{
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

