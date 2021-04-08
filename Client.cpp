/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mskinner <v.golskiy@ya.ru>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/31 18:01:21 by mskinner          #+#    #+#             */
/*   Updated: 2021/04/06 11:15:22 by mskinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(Socket *listen_sock) 
: _fd(-1), _port(0), _listen_sock(listen_sock) 
{
    _request = nullptr; // doesn't work in linux (?)
}

Client::Client(const Client &copy) {*this = copy;};

Client::~Client() {}

Client	Client::operator=(const Client &other) {
	if (this != &other) {
		_fd = other._fd;
		_port = other._port;
		_host = other._host;
		_address = other._address;
        _inet = other._inet;
        _listen_sock = other._listen_sock;
	}
	return (*this);
};

int Client::get_socket_fd()
{
    return _fd;
}

Request * Client::get_request()
{
    return _request;
}

bool Client::accept_connection()
{
    int addrlen = sizeof(_address);

    _fd = accept(_listen_sock->get_fd(), (struct sockaddr*)&_address, (socklen_t*)&addrlen);
    if (_fd == false)
        return (false);
    _host = _address.sin_family;
    _port = _address.sin_port;
    _inet = _address.sin_addr.s_addr;

    // TEST message:
	char buffer[1024] = {0};
	const char* hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 20\n\nServer-Test message!";
    read(_fd, buffer, 1024);
	std::cout << buffer << std::endl;
    send(_fd, hello, strlen(hello), 0);
	std::cout << "Hello message sent\n\n";
    // END of Test;

    if (fcntl(_fd, F_SETFL, O_NONBLOCK) < 0)
		exit_error(errno);
    return true; 
}

void Client::readRequest()
{
    // check length of request -> put into buf;
    // use recv(_fd, &buf, size(buf), 0);

    // simple read from client (less than or equal to 1024):
    int buf = 1024;
    char buffer[buf];
    if (recv(_fd, &buffer, buf, 0) == 0) // TODO: check (ret < 0)!
        std::cout << "Nothing to read!\n";
    _to_parse += buffer;
    _request->parse_request(_to_parse);
    // parse request;
}
