/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mskinner <v.golskiy@ya.ru>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/31 18:01:21 by mskinner          #+#    #+#             */
/*   Updated: 2021/04/19 15:04:48 by mskinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

void	Client::clear_request() {
	delete _request;
}

Client::Client(Socket *listen_sock) 
: _fd(-1), _status(Client::INIT), _port(0), _listen_sock(listen_sock), _request(NULL), _time_start(current_time()) {}

Client::~Client() {
	if (_fd != -1)
		close(_fd);
	clear_request();
} // TODO: close _fd; delete _request+_response
//response will be deleted on server side

int         Client::get_fd(){return _fd;}

int         Client::get_s_addr(){return _address.sin_addr.s_addr;}

Request*    Client::get_request(){return _request;}

int			Client::get_status() const {return _status;}

long		Client::get_start_time() const {return _time_start;}

void		Client::accept_connection()
{
    int addrlen = sizeof(_address);

	if ((_fd = accept(_listen_sock->get_fd(), (struct sockaddr*)&_address, (socklen_t*)&addrlen)) == -1)
		throw (errno);
    _host = _address.sin_family;
    _port = _address.sin_port;
    _inet = _address.sin_addr.s_addr;
    
    // TEST message:
	// char buffer[1024] = {0};
	// const char* hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 20\n\nServer-Test message!";
    // read(_fd, buffer, 1024);
	// std::cout << buffer << std::endl;
    // send(_fd, hello, strlen(hello), 0);
	// std::cout << "Hello message sent\n\n";
    // END of Test;

    if (fcntl(_fd, F_SETFL, O_NONBLOCK) < 0)
		throw (errno);
    _status = Client::ALIVE;
}

void Client::readRequest(const int i)
{	
    int		buf_size = BUFFER_SIZE;
    
    if (!_request)
        _request = new Request(this);
    /*
	** In case of chunk request we changing BUFFER_SIZE to chunk_size
	** if remain len < buffer size -> change buffer size
    */
    while (1)
    {
        if (_request->get_status() == Request::BODY_PARSE && _request->get_remain_len() > 0)
            buf_size = _request->get_remain_len() < BUFFER_SIZE ? _request->get_remain_len() : BUFFER_SIZE;
        char	buffer[buf_size + 1];
        memset(buffer, 0, buf_size);
        int		to_recieve = 0;
        to_recieve = recv(_fd, &buffer, buf_size, 0);
        if (!to_recieve)
            _status = Client::EMPTY;
        else if (to_recieve == -1)
            _request->parse_request(_to_parse, i);
        else {
            buffer[to_recieve] = '\0';
            _to_parse += buffer;
            if (_request->get_status() == Request::BODY_PARSE)
                _request->cut_remain_len(to_recieve);
            _request->parse_request(_to_parse, i);
        }
        if (_request->get_status() == Request::DONE || _request->get_status() == Request::BAD_REQ)
        {
            std::cout << "Status: " << _request->get_status() << std::endl;
            break ;
        }
    }
    if (_request->get_status() != Request::BAD_REQ)
	    _request->set_cgi_meta_vars(i);
}
