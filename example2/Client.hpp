//
// Created by Kanat Saribaew on 3/5/21.
//

#ifndef CLIENT_HPP
#define CLIENT_HPP
#include "RequestParser.hpp"
#include "ClassParser.hpp"
class Client {
private:
	int _fd;
	Http _http;
	RequestParser _parser_request;
	std::string _remote_addr;
	t_server_config_data _config;
public:
	const RequestParser &getParserRequest() const;
	std::string get_remote_addr();
	const Http &getHttp() const;
	void setHttp(const Http &http);
	int getFd() const;
	Client(int fd, const std::string &, const std::vector<std::string>&, const t_server_config_data &);
	void setFd(int fd);

	const std::pair<bool, size_t> &getMehtodSend() const;

	void setMehtodSend(const std::pair<bool, size_t> &mehtodSend);
};


#endif //CLIENT_HPP
