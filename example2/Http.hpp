//
// Created by Kanat Saribaew on 2/26/21.
//

#ifndef HTTP_HPP
#define HTTP_HPP

#include "system_include.hpp"

class Http {
private:
	std::string _status;
	std::string _body;
	std::map<std::string,std::string> _headMap;
	std::map<std::string,std::string> _start_line;
	std::vector<std::string>response;
	std::string _buffer;
	size_t _length;
	int _error_flag;
	bool _big_changus;
	bool _count_body;
	size_t size_of_body;
public:
	int i;
	bool getBigChangus() const;

	void setBigChangus(bool bigChangus);

	bool getCountBody() const;

	void setCountBody(bool countBody);

	size_t getSizeOfBody() const;

	void setSizeOfBody(size_t sizeOfBody);
	const std::map<std::string, std::string> &getHeadMap() const;

	void setHeadMap(const std::map<std::string, std::string> &headMap);

	const std::map<std::string, std::string> &getStartLine() const;

	void setStartLine(const std::map<std::string, std::string> &startLine);


	int getErrorFlag() const;

	void setErrorFlag(int errorFlag);

	size_t getLength() const;

	void setLength(size_t length);

	Http();

	const std::string &getStatus() const;

	void setStatus(const std::string &status);

	const std::string &getBody() const;

	void setBody(const std::string &body);

	const std::string &getBuffer() const;

	void setBuffer(const std::string &buffer);

	const std::vector<std::string> &getResponse() const;

	void setResponse(const std::vector<std::string> &response);
	void clear();
};


#endif //HTTP_HPP
