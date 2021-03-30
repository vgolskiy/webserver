#ifndef HTTP_HPP
# define HTTP_HPP

# include "webserv.hpp"

class Http
{
public:
	static std::string const methods[];
	static std::string const protocols[];
	static std::string const headers[];
	static bool isValidMethod(std::string const &method);
	static bool isValidProtocol(std::string const &protocol);
	static bool isValidHeader(std::string const &header);
	static std::string getRightCaseHeader(std::string const &header);
};

#endif
