/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Сlient.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mskinner <v.golskiy@yandex.ru>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/31 18:01:21 by mskinner          #+#    #+#             */
/*   Updated: 2021/03/31 18:01:43 by mskinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(Socket *listen_sock) 
: _listen_sock(listen_sock), _fd(-1), _port(0) {}

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

bool Client::accept_connection()
{
    int addrlen = sizeof(_address);

    _fd = accept(_listen_sock->get_fd(), (struct sockaddr*)&_address, (socklen_t*)&addrlen);
    if (_fd == false)
        return (false);
    _host = _address.sin_family;
    _port = _address.sin_port;
    _inet = _address.sin_addr.s_addr;

    if (fcntl(_fd, F_SETFL, O_NONBLOCK) < 0)
		exit_error(errno);
    return true; 
}
