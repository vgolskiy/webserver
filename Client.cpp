/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mskinner <v.golskiy@ya.ru>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/31 18:01:21 by mskinner          #+#    #+#             */
/*   Updated: 2021/05/13 14:27:37 by mskinner         ###   ########.fr       */
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
}

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

void Client::read_run_request(const int i) {
	int		buf_size = BUFFER_SIZE;

    if (!_request)
        _request = new Request(this, i);
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
        else if ((to_recieve == -1) && (_to_parse.length())) //prevention of parse circle with empty lines
            _request->parse_request(_to_parse);
        else if (to_recieve != -1) { //prevention of parse circle with empty lines
            buffer[to_recieve] = '\0';
            _to_parse += buffer;
            _request->parse_request(_to_parse);
        }
        if (_request->get_status() == Request::DONE || _request->get_status() == Request::BAD_REQ)
        {
            std::cout << "Status: " << _request->get_status() << std::endl;
			// check-print request - delete later,  turn bach return after BAD REQ
   			if (_request->get_status() == Request::BAD_REQ) {
   			    std::cout << "BAD REQUEST CLIENT: " << std::endl;
   			    _request->print_parsed_request();
   			}
   			if (_request->get_status() == Request::DONE) {
   			    _request->print_parsed_request();
   			    std::cout << "Body: " << _request->get_body() << std::endl;
   			}
            break ;
        }
    }
    if (_request->get_status() != Request::BAD_REQ) {
		_request->parse_script_file_name();
		_request->set_cgi_meta_vars();
		if (_request->get_script_name())
			_request->run_cgi_request();
		send(_fd, get_request()->get_response().c_str(), get_request()->get_response().length(), 0);
	}
}
