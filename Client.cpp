/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mskinner <v.golskiy@ya.ru>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/31 18:01:21 by mskinner          #+#    #+#             */
/*   Updated: 2021/06/03 15:40:48 by mskinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

void	Client::clear_client(void) {
	delete _request;
	delete _response;
}

Client::Client(Socket *listen_sock) 
: _fd(-1), _status(Client::INIT), _port(0), _listen_sock(listen_sock), _to_parse(""),
_request(NULL), _response(NULL), _time_start(current_time()) {}

Client::~Client() {
	if (_fd != -1)
		close(_fd);
	clear_client();
}

int         Client::get_fd(void) { return (_fd); }

int         Client::get_s_addr(void) { return (_address.sin_addr.s_addr); }

Request*    Client::get_request(void) {return (_request); }

Response*	Client::get_response(void) { return (_response); }

void		Client::set_response(t_server *server) {
	if (!_response)
		_response = new Response(server, _request);

	_response->create_response();
};

int			Client::get_status(void) const { return (_status); }

void		Client::set_status(int status) { _status = status; }

long		Client::get_start_time(void) const {return (_time_start); }

void		Client::set_starttime(void) { _time_start = current_time(); }

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
		error_message("Request timed out");
	}
}

void		Client::print_request_debug(void) {
	std::cout << "Status: " << _request->get_status() << std::endl;
	if (_request->get_status() == Request::BAD_REQ) {
		std::cout << "BAD REQUEST CLIENT: " << std::endl;
		_request->print_parsed_request();
   	}
	if (_request->get_status() == Request::DONE) {
   		_request->print_parsed_request();
   		std::cout << "Body length: " << _request->get_body().length() << std::endl;
   	}
}

void		Client::read_run_request(const int i) {
	int		buf_size = BUFFER_SIZE;
	int		to_receive = 0;

	if (!_request)
		_request = new Request(this, i);
    /*
	** In case of chunk request we changing BUFFER_SIZE to chunk_size
	** if remain len < buffer size -> change buffer size
    */
	if (_request->get_status() == Request::BODY_PARSE && _request->get_remain_len() > 0)
		buf_size = _request->get_remain_len() < BUFFER_SIZE ? _request->get_remain_len() : BUFFER_SIZE;
	char	buffer[buf_size + 1];

	memset(buffer, 0, buf_size);
	to_receive = recv(_fd, &buffer, buf_size, 0);
	if (!to_receive && !_to_parse.length()) {
		_status = Client::EMPTY;
		return ;
	}
	//preventing empty lines parcing try
	if ((to_receive == -1) && (_to_parse.length()))
		_request->parse_request(_to_parse);
	else if (to_receive != -1) {
		if (to_receive) {
			buffer[to_receive] = '\0';
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
	//verify_request_timeout(30); //TESTING
	//Need to read request till the end even if it is meaningless to prevent connection reset by peer
	//https://stackoverflow.com/questions/1434451/what-does-connection-reset-by-peer-mean
/*	if ((_request->get_status() == Request::BAD_REQ)
		|| (_request->get_status() == Request::CHUNK_DONE)) {
		//preventing "slamming the phone back on the hook" effect
		usleep(1000);
		to_receive = recv(_fd, &buffer, buf_size, 0);
		if ((to_receive <= 0) && _request->get_status() == Request::CHUNK_DONE)
			_request->set_request_status(Request::DONE);
	}
	*/
	//TESTING
	if (_request->get_status() == Request::DONE 
		|| _request->get_status() == Request::BAD_REQ)
        print_request_debug();
}
