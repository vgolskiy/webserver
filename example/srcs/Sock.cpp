#include "Sock.hpp"

Sock::Sock(int domain, int type, int protocol):
	_domain(domain),
	_sockfd(-1)
{
	this->_sockfd = socket(domain, type, protocol);
	if (this->_sockfd == -1)
		throw ErrnoException();
	if (fcntl(this->_sockfd, F_SETFL, O_NONBLOCK) == -1)
		throw ErrnoException();
}

Sock::~Sock()
{
	if (this->_sockfd > 2)
		close(this->_sockfd);
}

void Sock::bindSocket(int port, int addr) const
{
	struct sockaddr_in addr_in;
	addr_in.sin_family = this->_domain;
	addr_in.sin_port = htons(port);
	addr_in.sin_addr.s_addr = addr;

	int opt = 1;
	int opt1 = 1;
	int opt2 = BUFFERSIZE;
	if (setsockopt(this->_sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) < 0)
		throw ErrnoException();
	if (setsockopt(this->_sockfd, SOL_SOCKET, SO_KEEPALIVE, &opt1, sizeof(int)) < 0)
		throw ErrnoException();
	if (setsockopt(this->_sockfd, SOL_SOCKET, SO_RCVBUF, &opt2, sizeof(int)) < 0)
		throw ErrnoException();
	if (bind(this->_sockfd, (struct sockaddr*) &addr_in, sizeof(addr_in)) < 0)
		throw ErrnoException();
}

void Sock::listenTo(int qlen) const
{
	if (listen(this->_sockfd, qlen) == -1)
		throw ErrnoException();
}

int Sock::getSockfd() const
{
	return this->_sockfd;
}
