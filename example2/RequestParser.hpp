//
// Created by Kanat Saribaew on 3/5/21.
//

#ifndef REQUESTPARSER_HPP
#define REQUESTPARSER_HPP

#include "Http.hpp"

class RequestParser {
public:
	void parser(Http &http);
	int getErrorFlag() const;

	const std::string &getStatus() const;

	void setStatus(const std::string &status);

	const std::map<std::string, std::string> &getHeadMap() const;

	void setHeadMap(const std::map<std::string, std::string> &headMap);

	const std::map<std::string, std::string> &getStartLine() const;

	void setStartLine(const std::map<std::string, std::string> &startLine);

	const std::string &getBuffer() const;

	void setBuffer(const std::string &buffer);

	const std::list<std::string> &getTokens() const;

	void setTokens(const std::list<std::string> &tokens);

	void setErrorFlag(int errorFlag);

	RequestParser(const std::vector<std::string>&, const std::vector<std::string>&, int);
	const std::string &getBody() const;
	void clear();
private:
	std::string _status;
	std::vector<std::string> _supported_methods;
	std::vector<std::string> _server_names;
	std::map<std::string, std::string> _headMap;
	std::map<std::string, std::string> _start_line;
	std::string _buffer;
	std::string _body;
	std::list<std::string> _tokens;
	int _error_flag;
	int _parser_flag;
	size_t _max_body_size;
	size_t _contet_length;
	bool _big_changus;
	bool _count_body;
	size_t size_of_body;

	bool _check_validation_start_line();

	bool _parser_tokens();

	bool _parser_start_line();

	bool _parser_uri();

	bool _parser_head();

	void _parser_location(std::string &);

	void  _body_chunked();

	void _to_upper(std::string &str);

	void _to_lower(std::string &str);

	size_t _find(std::string &s1, std::string &s2);

	size_t _findi(std::string &s1, std::string &s2, size_t n);
	size_t _to_int(std::string str);

};


#endif //REQUESTPARSER_HPP
