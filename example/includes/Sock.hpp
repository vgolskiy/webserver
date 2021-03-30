#ifndef SOCK_HPP
# define SOCK_HPP

# include "webserv.hpp"

class Sock
{
public:
	Sock(int domain, int type, int protocol);
	~Sock();
	void bindSocket(int port, int addr) const;
	void listenTo(int qlen) const;
	int getSockfd() const;
private:
	int _domain;
	int _sockfd;
	std::string _buffStr;
};

#endif
