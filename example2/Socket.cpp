//
// Created by Kanat Saribaew on 2/24/21.
//

#include "Socket.hpp"
const char * Socket::BadAddress::what() const throw() {
	return "bad address";
}
const char * Socket::SocketError::what() const throw() {
	return "_socket error";
}
void Socket::listen_socket() {
	int optval = 1;
	if ((_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		throw Socket::SocketError();
	if (setsockopt(_sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) == -1){
		throw Socket::SocketError();
	}
	_sock_addr.sin_family = AF_INET;
	if ((_sock_addr.sin_addr.s_addr = inet_addr(_addres.c_str())) == INADDR_NONE && _addres != "255.255.255.255"){
		throw Socket::BadAddress();
	}
	_sock_addr.sin_port = static_cast<unsigned short>((atoi(_port.c_str()) << 8) | (atoi(_port.c_str()) >> 8));
	if (bind(_sock, reinterpret_cast<sockaddr *>(&_sock_addr), _size_sock_addr) == -1){
		throw Socket::SocketError();
	}
	if (listen(_sock, SOMAXCONN) == -1){
		throw Socket::SocketError();
	}
}
int Socket::accept_socket(sockaddr_in *addr, socklen_t *size) {
	int new_socket;
	if ((new_socket = accept(_sock, reinterpret_cast<sockaddr *>(addr), size)) < 0){
		throw Socket::SocketError();
	}
	return new_socket;
}
void Socket::connect_socket() {
	if ((_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		throw Socket::SocketError();
	_sock_addr.sin_family = AF_INET;
	if ((_sock_addr.sin_addr.s_addr = inet_addr(_addres.c_str())) == INADDR_NONE && _addres != "255.255.255.255"){
		throw Socket::BadAddress();
	}
	_sock_addr.sin_port = static_cast<unsigned short>((atoi(_port.c_str()) << 8) | (atoi(_port.c_str()) >> 8));
	if (connect(_sock, reinterpret_cast<sockaddr *>(&_sock_addr), _size_sock_addr) != 0){
		throw Socket::SocketError();
	}
}
void Socket::close_socet(int sock) {
	 close(sock);
}

const std::string &Socket::getAddres() const {
	return _addres;
}

void Socket::setAddres(const std::string &addres) {
	_addres = addres;
}

const std::string &Socket::getPort() const {
	return _port;
}

void Socket::setPort(const std::string &port) {
	_port = port;
}
size_t Socket::response(int sock_fd, const std::string &val, size_t size) {
	return send(sock_fd, val.c_str(), size, 0);
}
bool  Socket::receive(int sock_fd, Http &http) {
	char *buf = new char[http.getLength() + 1];
//	bzero(buf, http.getLength() + 1);
	std::string str = http.getBuffer();
	int i = recv(sock_fd, buf, http.getLength(), 0);
	if (i == 0){
		delete[] buf;
		return false;
	}else if (i > 0){
		buf[i] = '\0';
		str += std::string(buf);
		http.setBuffer(str);
		delete[] buf;
		return true;
	}
	delete [] buf;
	return false;
}
Socket::Socket(const std::string &addres, const std::string &port) : _addres(addres), _port(port){
	_size_sock_addr = sizeof (sockaddr_in);

}

Socket::Socket() {_size_sock_addr = sizeof (sockaddr_in);}

int Socket::getSock() const {
	return _sock;
}
