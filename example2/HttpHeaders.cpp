//
// Created by Kanat Saribaew on 3/12/21.
//

#include "HttpHeaders.hpp"

//std::vector<std::string> HttpHeaders::create_headers(Http &http, std::map<std::string, std::string>m, t_locations &loc, t_server_config_data &conf) {
//    std::map<std::string, std::string>star_line = http.getStartLine();
//    std::map<std::string, std::string>map_head = http.getHeadMap();
//    std::string header;
////	header +=
//    return std::vector<std::string>();
//}

HttpHeaders::HttpHeaders(const std::map<std::string, std::string> &mimeTypes) :_mime_types(mimeTypes) {
	std::string s1("100 101 102 103 200 201 202 203 204 205 206 300 301 302 303 304 305 306 307 308 400 401 402 403 404 405 406 407 408 409 410 411 412 413 414 415 416 417 500 501 502 503 504 505 ");
	std::string s2("Continue\nSwitching Protocol\nProcessing\nEarly Hints\nOK\nCreated\nAccepted\nNon-Authoritative Information\nNo Content\nReset Content\nPartial Content\nMultiple Choice\nMoved Permanently\nFound\nSee Other\nNot Modified\nUse Proxy\nSwitch Proxy\nTemporary Redirect\nPermanent Redirect\nBad Request\nUnauthorized\nPayment Required\nForbidden\nNot Found\nMethod Not Allowed\nNot Acceptable\nProxy Authentication Required\nRequest Timeout\nConflict\nGone\nLength Required\nPrecondition Failed\nRequest Entity Too Large\nRequest-URI Too Long\nUnsupported Media Type\nRequested Range Not Satisfiable\nExpectation Failed\nInternal Server Error\nNot Implemented\nBad Gateway\nService Unavailable\nGateway Timeout\nHTTP Version Not Supported\n");
	std::vector<std::string> v1 = ft_split_string_to_vector(s1, ' ');
	std::vector<std::string> v2 = ft_split_string_to_vector(s2, '\n');
	for (size_t i = 0; i < v2.size(); i++)
	{
		v2[i] = "HTTP/1.1 " + v1[i] + " " + v2[i] + "\r\n";
		_status.insert(std::pair<std::string, std::string>(v1[i], v2[i]));
	}
}

const std::string HttpHeaders::get_date(time_t t) const{
    struct tm info;
    char buffer[64];

    std::string s = std::to_string(t);
    strptime(s.c_str(), " %s ", &info);
    strftime (buffer, sizeof(buffer), "%a, %d %b %Y %X %Z", &info);
    std::string str = buffer;
    return str;
}
std::string HttpHeaders::get_server() const {
    return std::string("Server: web_server\r\n");
}

std::string HttpHeaders::get_content_type(const std::string &extensions) const {
    std::map<std::string, std::string>::const_iterator  it;
    if ((it = _mime_types.find(extensions)) != _mime_types.end()){
        return std::string("Content-Type: " + it->second + "\r\n");
    }
    return std::string("Content-Type: text/plain\r\n");
}

std::string HttpHeaders::get_last_modified(const std::string &str) {
    struct stat	buf;
    int			result;

    result = stat(str.c_str(), &buf);
    if (result == 0){
        return "Last-Modified: " + get_date(buf.st_mtimespec.tv_sec) + "\r\n";
    }
    return  std::string();
}

std::string HttpHeaders::get_server_date() const {
	struct timeval tv;
	time_t t;
	gettimeofday(&tv, NULL);
	t = tv.tv_sec;
    return "Date: "+get_date(t) + "\r\n";
}

std::string HttpHeaders::generate_headers(std::map<std::string, std::string> &m) {
	std::string headers;
	headers = _status[m["status"]];
	headers += get_server();
	headers += get_server_date();
	headers += get_content_type(m["type"]);
	headers += "Content-length: " + m["length"] + "\r\n";
	if (m.find("file") != m.end())
		headers += get_last_modified(m["file"]);
	return headers;
}

const std::string &HttpHeaders::get_status(const std::string &code_status) {
	return _status[code_status];
}

std::string HttpHeaders::get_content_length(const std::string &len) const {
	return std::string("Content-length: " + len + "\r\n");
}

std::string HttpHeaders::generate_special_headers(int code_error, t_locations &loc, std::string l) {
	if (code_error >= 300 && code_error < 400){
		return std::string("Location: " + l + "/" + "\r\n");
	}else if (code_error == 401){
		return  std::string ("WWW-Authenticate: " + loc.auth_data.AuthType + " realm=\"Access to the staging site\", charset=\"UTF-8\"\r\n");
	}else if (code_error == 405){
		std::string str = "Allow: ";
		for (size_t i = 0; i < loc.method.size() ; ++i) {
			str+= loc.method[i];
			if (i + 1 < loc.method.size())
				str += ",";
		}
		return str + "\r\n";
	}
	return std::string();
}
