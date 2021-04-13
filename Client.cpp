/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mskinner <v.golskiy@yandex.ru>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/31 18:01:21 by mskinner          #+#    #+#             */
/*   Updated: 2021/04/13 11:27:52 by mskinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

void	Client::clear_request() {
	delete _request;
}

Client::Client(Socket *listen_sock) 
: _fd(-1), _status(Client::INIT), _port(0), _listen_sock(listen_sock),
_to_parse(""), _request(NULL) {}

Client::~Client() {
	if (_fd != -1)
		close(_fd);
	clear_request();
} // TODO: close _fd; delete _request+_response
//response will be deleted on server side

int         Client::get_socket_fd(){return _fd;}

int         Client::get_s_addr(){return _address.sin_addr.s_addr;}

Request*    Client::get_request(){return _request;}

bool Client::accept_connection()
{
    int addrlen = sizeof(_address);

	if ((_fd = accept(_listen_sock->get_fd(), (struct sockaddr*)&_address, (socklen_t*)&addrlen)) == -1)
		exit_error(errno); // is it better to throw exception?
		//just doing the same behavior in all cases
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
    _status = Client::ALIVE;

    return true;
}

void Client::readRequest()
{
    int buf_size = BUFFER_SIZE;
    int to_recieve;

    // if no request - create one for this client
    if (!_request)
        _request = new Request(this);

    // if remain len < buffer size -> change buffer size
    if (_request->get_remain_len() < buf_size)
        buf_size = _request->get_remain_len();
    char buffer[buf_size + 1];
    to_recieve = recv(_fd, &buffer, buf_size, 0);

    // in nonblocking case -1 is returned if no messages are available
    switch(to_recieve)
    {
        case 0:
            _status = Client::EMPTY;
        case -1:
            _request->parse_request(_to_parse); // parse what left
        default:
            buffer[to_recieve] = '\0';
            _to_parse += buffer;
            _request->parse_request(_to_parse); // added conditions
            // cut what we've parsed in case if we parse body
            if (_request->get_status() == Request::BODY_PARSE)
                _request->cut_remain_len(to_recieve);
    }
    _request->parse_request(_to_parse);
    // parse request;
}
