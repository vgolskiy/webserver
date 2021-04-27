//
// Created by Kanat Saribaew on 2/24/21.
//

#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <fstream>
#include "system_include.hpp"
#include "Http.hpp"
class Socket {
public:
	Socket();
	Socket(const std::string &addres, const std::string &port);
	void listen_socket();
	int accept_socket(sockaddr_in*, socklen_t *);
	void connect_socket();
	const std::string &getAddres() const;
	void setAddres(const std::string &addres);
	const std::string &getPort() const;
	void setPort(const std::string &port);
	void close_socet(int sock);

	int getSock() const;

	bool receive(int sock_fd, Http &host);

	size_t response(int sock_fd, const std::string &val, size_t);

	class BadAddress : public std::exception{
	virtual const char * what() const throw();
	};

	class SocketError : public std::exception{
		virtual const char * what() const throw();
	};

	class BadPort : public std::exception{
		virtual const char * what() const throw();
	};
private:
	std::string _addres;
	std::string _port;
	int _sock;
	sockaddr_in _sock_addr;
	socklen_t _size_sock_addr;
};


#endif //SOCKET_HPP
