//
// Created by Kanat Saribaew on 3/5/21.
//

#include "RequestParser.hpp"
#include "parser_config.hpp"

void RequestParser::_to_upper(std::string &str) {
	for (size_t i = 0; i < str.length(); ++i) {
		str[i] = std::toupper(str[i]);
	}
}
void RequestParser::_to_lower(std::string &str) {
	for (size_t i = 0; i < str.length(); ++i) {
		str[i] = std::tolower(str[i]);
	}
}
size_t RequestParser::_find(std::string &s1, std::string &s2) {
	for (size_t i = 0, pos = 0; i < s2.length(); ++i){
		if ((pos = s1.find_first_of(s2[i])) != std::string::npos){
			return pos;
		}
	}
	return std::string::npos;
}
size_t RequestParser::_findi(std::string &s1, std::string &s2, size_t n) {
	size_t  i = 0;
	for (; i < n; ++i){
		if (s1[i] != s2[i]){
			if ((isalpha(s1[i]) && isalpha(s2[i])) && (tolower(s1[i]) == tolower(s2[i]))) {
				continue;
			}
			return std::string::npos;
		}
	}
	return i;
}

bool RequestParser::_parser_head() {
	std::string header;
	std::string value;
	std::string invalid_character = "\"(),/:;<=>?@[\\]{} ";
	std::list<std::string>::iterator it = _tokens.begin();
//	for (; it != _tokens.end(); ++it){
//
//	}
	it  = _tokens.begin();
	for (; it != _tokens.end(); ++it){
		size_t pos;
		if ((pos = it->find(":")) != std::string::npos) {
			header = it->substr(0, pos);
			value = it->substr(pos + 1, it->length());
		}
		if (_find(header, invalid_character) != std::string::npos){
			_status = "error";
			_error_flag = 400;
			return false;
		} else{
			_to_lower(header);
			if (header != "authorization")
				_to_lower(value);
			value =  ft_trim_spases(value);
			std::pair<std::map<std::string, std::string>::iterator, bool>p = _headMap.insert(std::pair<std::string, std::string>(header, value));
			if (!p.second){
				if (header == "host" || header == "content-length"){
					_status = "error";
					_error_flag = 400;
					return false;
				}
				p.first->second = value;
			}
		}
	}
	std::map<std::string, std::string>::iterator it_m;

	if ((it_m = _headMap.find("transfer-encoding")) != _headMap.end()){
		if (it_m->second != "chunked"){
			_status = "error";
			_error_flag = 501;
			return false;
		}
		_status = "read_body_chunked";
	} else if ((it_m = _headMap.find("content-length")) != _headMap.end()){
		_contet_length = atol(it_m->second.c_str());
		if (_max_body_size < _contet_length || _contet_length < 0){
			_status = "error";
			_error_flag = 413;
			return false;
		}
		_status = "read_body_content_length";
	}
	return true;
}
void RequestParser::_parser_location(std::string &uri){
	std::string location;
	size_t pos = uri.find("?");
	location = pos != std::string::npos ? uri.substr(0, pos) : uri;
	if (pos != std::string::npos){
		uri.erase(0, pos + 1);
		_start_line["query_string"] = uri;
	}
	pos = location.find_last_of("/");
	_start_line["location"] = location.substr(0, pos + 1);
	location.erase(0, pos + 1);
	_start_line["source"] = location;
}
bool RequestParser::_parser_uri() {
	std::string delimetr = "http://";
	std::string uri = _start_line["uri"];
	std::string host;
	if (_findi( uri, delimetr, delimetr.length()) != std::string::npos) {
		uri.erase(0, delimetr.length());
		size_t i = 0;
		for (; i < uri.length(); ++i){
			if (uri[i] == '/' || uri[i] == '?'){
				break;
			}
		}
		_start_line["host"] = uri.substr(0, i);
		if (uri[i] == '/'){
			uri.erase(0, i);
			_parser_location(uri);
		}else if (uri[i] == '?'){
			uri.erase(0, i + 1);
			_start_line["location"] = "/";
			_start_line["source"] = "";
			_start_line["query_string"] = uri;
		} else {
			_start_line["location"] = "/";
			_start_line["source"] = "";
		}
	}else if (uri[0] == '/') {
		_start_line["host"] = _server_names.front();
		_parser_location(uri);
	}else{
		_status = "error";
		_error_flag = 400;
		return false;
	}
	host = _start_line["host"];
	for (size_t i = 0; i < _server_names.size(); ++i) {
		if (host == _server_names[i])
			return true;
	}
	_status = "error";
	_error_flag = 400;
	return false;
}

bool RequestParser::_check_validation_start_line() {
	size_t pos;
	if (std::find(_supported_methods.begin(), _supported_methods.end(), _start_line["method"]) == _supported_methods.end()){
		_status = "error";
		_error_flag = 501;
		return false;
	}
	if (std::find(_server_names.begin(), _server_names.end(), _start_line["host"]) == _server_names.end() || (pos = _start_line["http_version"].find("HTTP/")) != 0){
		_status = "error";
		_error_flag = 400;
		return false;
	}
	if ((pos = _start_line["http_version"].find("1.1", 5)) == std::string::npos){
		_status = "error";
		_error_flag = 505;
		return false;
	}
	if (_start_line["http_version"].length() != 8){
		_status = "error";
		_error_flag = 400;
		return false;
	}
	_start_line["path_info"] = _start_line.find("location")->second + _start_line.find("source")->second;
	return true;
}
bool RequestParser::_parser_start_line() {
	size_t pos = 0;
	std::list<std::string> tokens;
	while ((pos = _tokens.front().find(" ")) != std::string::npos) {
		tokens.push_back(_tokens.front().substr(0, pos));
		_tokens.front().erase(0, pos + 1);
	}
	tokens.push_back(_tokens.front().substr(0, _tokens.front().length()));
	_tokens.pop_front();
	if (tokens.size() != 3) {
		_status = "error";
		_error_flag = 400;
		return false;
	}
	std::list<std::string>::iterator it = tokens.begin();
	_start_line["method"] = *it++;
	_start_line["uri"] = *it++;
	_start_line["http_version"] = *it++;
	if (!_parser_uri())
		return false;
	return _check_validation_start_line();
}

bool RequestParser::_parser_tokens() {
	size_t pos = 0;
	std::string token;
	std::string delimiter = "\r\n\r\n";

	while (_buffer[pos] == '\r' || _buffer[pos] == '\n') {
		pos++;
	}
	_buffer.erase(0, pos);
	pos = 0;
	if ((pos = _buffer.find(delimiter)) != std::string::npos) {
		token += _buffer.substr(0, pos);
		_buffer.erase(0, pos + delimiter.length());
		delimiter = "\r\n";
		while ((pos = token.find(delimiter)) != std::string::npos) {
			_tokens.push_back(token.substr(0, pos));
			token.erase(0, pos + delimiter.length());
		}
		_tokens.push_back(token.substr(0, token.length()));
		if (_tokens.size() < 2) {
			_tokens.clear();
			_buffer.clear();
			_status = "error";
			_error_flag = 400;
			return false;
		}
		_status = "execute";
		return true;
	}
	_status = "read_header";
	return false;
}

int RequestParser::getErrorFlag() const {
	return _error_flag;
}
size_t RequestParser::_to_int(std::string str) {
	std::stringstream stream;
	size_t n;
	stream << std::hex<<str;
	stream >> n;
	return n;
}

void RequestParser::clear() {

	_headMap.clear();
	_start_line.clear();
	_error_flag = 0;
	_buffer.clear();
	_body.clear();
	_status.clear();
	_tokens.clear();
	_contet_length = READ_BUFFER;
	_big_changus = false;
}

void RequestParser::parser(Http &http) {
	_buffer = http.getBuffer();
	if (http.getStatus() == "read_header") {
		if (!_parser_tokens() || !_parser_start_line() || !_parser_head()) {
			if (_status != "error"){
				return;
			}
			http.setErrorFlag(_error_flag);
			http.setStatus(_status);
			clear();
			return;
		}
		if (_buffer.length() > 0 && (_headMap.find("content-length") == _headMap.end() && _headMap.find("transfer-encoding") == _headMap.end())){
			_status = "error";
			_error_flag = 411;
			http.setErrorFlag(_error_flag);
			http.setStatus(_status);
			clear();
		}
		http.setStatus(_status);
		http.setErrorFlag(_error_flag);
		http.setHeadMap(_headMap);
		http.setStartLine(_start_line);
		http.setLength(_contet_length);
		http.setBuffer(_buffer);
		if (_status == "execute"){
			clear();
		}
		_tokens.clear();
		_status.clear();
		_error_flag = 0;
	}if (http.getStatus() == "read_body_chunked" && !_buffer.empty()){
		_count_body = http.getCountBody();
		_big_changus = http.getBigChangus();
		size_of_body = http.getSizeOfBody();

		_body = http.getBody();
		_body_chunked();
		http.setBuffer(_buffer);
		http.setStatus(_status);
		http.setBody(_body);
		http.setCountBody(_count_body);
		http.setBigChangus(_big_changus);
		http.setSizeOfBody(size_of_body);
		clear();
	}else if (http.getStatus() == "read_body_content_length"){
		if (http.getLength() > _buffer.length()){
			_body = _buffer;
		}else {
			_body = _buffer.substr(0, http.getLength());
		}
		_status = "execute";
		http.setStatus(_status);
		http.setBody(_body);
		clear();
	}
}

const std::string &RequestParser::getStatus() const {
	return _status;
}

void RequestParser::setStatus(const std::string &status) {
	_status = status;
}

const std::map<std::string, std::string> &RequestParser::getHeadMap() const {
	return _headMap;
}

void RequestParser::setHeadMap(const std::map<std::string, std::string> &headMap) {
	_headMap = headMap;
}

const std::map<std::string, std::string> &RequestParser::getStartLine() const {
	return _start_line;
}

void RequestParser::setStartLine(const std::map<std::string, std::string> &startLine) {
	_start_line = startLine;
}

const std::string &RequestParser::getBuffer() const {
	return _buffer;
}

void RequestParser::setBuffer(const std::string &buffer) {
	_buffer = buffer;
}

const std::list<std::string> &RequestParser::getTokens() const {
	return _tokens;
}

void RequestParser::setTokens(const std::list<std::string> &tokens) {
	_tokens = tokens;
}

void RequestParser::setErrorFlag(int errorFlag) {
	_error_flag = errorFlag;
}

RequestParser::RequestParser(const std::vector<std::string> &server_name, const std::vector<std::string> &supported_methods, int max_size) : _status(), _supported_methods(supported_methods), _server_names(server_name),
																																			 _headMap(), _start_line(), _buffer(), _tokens(),
																																			 _error_flag(), _parser_flag(), _max_body_size(max_size), _contet_length(READ_BUFFER), _big_changus(false), size_of_body(0) {}

const std::string &RequestParser::getBody() const {
	return _body;
}





