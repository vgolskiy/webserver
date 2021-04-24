/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mskinner <v.golskiy@ya.ru>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/31 18:01:21 by mskinner          #+#    #+#             */
/*   Updated: 2021/04/20 16:32:13 by mskinner         ###   ########.fr       */
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

void Client::read_run_request(const int i)
{
	int	buf_size = BUFFER_SIZE;
    
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
            std::cout << "REQUEST TO PARSE\n";
            std::cout << _to_parse << std::endl;
            if (_request->get_status() == Request::BODY_PARSE)
                _request->cut_remain_len(to_recieve);
            _request->parse_request(_to_parse, i);
        }
        if (_request->get_status() == Request::DONE || _request->get_status() == Request::BAD_REQ || _request->get_status() == Request::PNG)
        {
            std::cout << "Status: " << _request->get_status() << std::endl;
            break ;
        }
    }
    // if (_request->get_status() != Request::BAD_REQ) {
	// 	_request->parse_script_file_name(i);
	//     _request->set_cgi_meta_vars(i);
	// 	_request->run_cgi_request();
	// }
    if (Request::DONE)
    {
        std::string response = "HTTP/1.1 404 Not Found\r\nDate: Sun, 18 Oct 2012 10:36:20 GMT\r\nServer: Apache/2.2.14 (Win32)\r\nContent-Type: text/html; image/png\r\n\r\n<html><head><title>404 Not Found</title></head><body><h1>Not Found</h1><p>The requested resource could not be found. As well as your sex life.</p><p><img src=\"/content/error_images/error_400.png\" width=60%></p></body></html>\r\n\r\n";
        // std::string response = "HTTP/1.1 404 Not Found\r\nDate: Sun, 18 Oct 2012 10:36:20 GMT\r\nServer: Apache/2.2.14 (Win32)\r\nContent-Type: text/html; multipart/form-data; charset=iso-8859-1\r\n\r\n/Users/rmanfred/School21/web_collab/content/error/4xx/404.html\r\n\r\n";
        send(_fd, response.c_str(), response.length(), 0);
    }
    if (Request::PNG)
    {
        int				was_read;
	    int				fd;
	    char			*buf = NULL;
        std::string		res = "";

        std::string response = "HTTP/1.1 200 OK\r\nDate: Sun, 18 Oct 2012 10:36:20 GMT\r\nServer: Apache/2.2.14 (Win32)\r\nContent-Type: text/html\r\n\r\n";

        fd = open(PICTURE, O_RDONLY);
        if (fd == -1)
            std::cout << "Fd cannot be opened\n";
        if (!(buf = (char *)ft_calloc(BUFFER_SIZE + 1, sizeof(char)))) {
            close(fd);
            throw (-1);
        }
        while ((was_read = read(fd, buf, BUFFER_SIZE)) > 0)
            res.append(buf);
        close(fd);
        free(buf);
        response += res;
        std::cout << "Res length: " << res.length() << std::endl;
        response += "\r\n\r\n";
        ssize_t p;
        p = send(_fd, response.c_str(), response.length(), 0);
        std::cout << "Send returns: " << p << std::endl;
    }
}
