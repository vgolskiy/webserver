/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mskinner <v.golskiy@yandex.ru>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/31 17:35:07 by mskinner          #+#    #+#             */
/*   Updated: 2021/03/31 18:01:43 by mskinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "Config.hpp"
# include "Request.hpp"

class Config;
class Socket;
class Request;
class Client
{
private:
	int					_fd;
	unsigned short		_port;
	std::string			_host;
	struct sockaddr_in	_address;
	in_addr_t			_inet;
	Socket				*_listen_sock;
	std::string			_to_parse;
	Request				*_request;

public:
	Client(Socket *listen_sock);
	Client(const Client &copy);
	~Client();
	Client	operator=(const Client &other);

	int get_socket_fd();

	bool accept_connection();
	void readRequest();
};
