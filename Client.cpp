/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mskinner <v.golskiy@ya.ru>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/31 18:01:21 by mskinner          #+#    #+#             */
/*   Updated: 2021/05/21 10:37:14 by mskinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

void	Client::clear_request(void) {
	delete _request;
}

Client::Client(Socket *listen_sock) 
: _fd(-1), _status(Client::INIT), _port(0), _listen_sock(listen_sock), _request(NULL), _time_start(current_time()) {}

Client::~Client() {
	if (_fd != -1)
		close(_fd);
	clear_request();
}

int         Client::get_fd(void) { return (_fd); }

int         Client::get_s_addr(void) { return (_address.sin_addr.s_addr); }

Request*    Client::get_request(void) {return (_request); }

Response*	Client::get_response(void) { return (_response); }

void		Client::set_response(Response* r) {_response = r;};

int			Client::get_status(void) const { return (_status); }

long		Client::get_start_time(void) const {return (_time_start); }

void		Client::accept_connection(void) {
    int addrlen = sizeof(_address);

	if ((_fd = accept(_listen_sock->get_fd(), (struct sockaddr*)&_address, (socklen_t*)&addrlen)) == -1)
		throw (errno);
    _host = _address.sin_family;
    _port = _address.sin_port;
    _inet = _address.sin_addr.s_addr;
    if (fcntl(_fd, F_SETFL, O_NONBLOCK) < 0)
		throw (errno);
    _status = Client::ALIVE;
}

void		Client::verify_request_timeout(int timeout_client) {
	if ((_request->get_status() == Request::CHUNK) || (_request->get_status() == Request::CHUNK_DATA)) 
		//need some time to fill the line manually: 30 seconds
		timeout_client *= 3;
	if ((current_time() - _time_start) > timeout_client * 1000) {
		_request->set_request_status(Request::BAD_REQ);
		_request->set_status_code(408);
	}
}

void Client::read_run_request(const int i) {
	int		buf_size = BUFFER_SIZE;
	int		to_recieve;

    if (!_request)
        _request = new Request(this, i);
    /*
	** In case of chunk request we changing BUFFER_SIZE to chunk_size
	** if remain len < buffer size -> change buffer size
    */
    while (true)
    {
        if (_request->get_status() == Request::BODY_PARSE && _request->get_remain_len() > 0)
            buf_size = _request->get_remain_len() < BUFFER_SIZE ? _request->get_remain_len() : BUFFER_SIZE;
        char	buffer[buf_size + 1];
        memset(buffer, 0, buf_size);
        to_recieve = 0;
        to_recieve = recv(_fd, &buffer, buf_size, 0);
        if (!to_recieve)
            _status = Client::EMPTY;
        if ((to_recieve == -1) && (_to_parse.length())) //prevention of parse circle with empty lines
            _request->parse_request(_to_parse);
        else if (to_recieve != -1) { //prevention of parse circle with empty lines
			if (to_recieve) {
            	buffer[to_recieve] = '\0';
            	_to_parse += buffer;
				//!!!to prevent timeout during manual input/big file reading we placing start time update here!!!
				_time_start = current_time();
			}
			if (_to_parse.length())
            	_request->parse_request(_to_parse);
			else
				_request->set_request_status(Request::BAD_REQ);
        }
		//10 seconds for request parse
		verify_request_timeout(10);
        if (_request->get_status() == Request::DONE || _request->get_status() == Request::BAD_REQ)
        {
            std::cout << "Status: " << _request->get_status() << std::endl;
			// TESTING check-print request - delete later,  turn bach return after BAD REQ
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
}
