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

/*
Statuses:
	INIT - in a constructor;
	ALIVE - once connection accepted
	DONE - once response sent and nothing left (send returns >= 0) + TODO: delete request+response
	DEAD - if recv returns 0 or if send returns (<0)
*/

enum status
{
	INIT,
	ALIVE,
	DONE,
	EMPTY
};

private:
	int					_fd;
	int					_status;
	unsigned short		_port;
	std::string			_host;
	struct sockaddr_in	_address;
	in_addr_t			_inet;
	Socket				*_listen_sock;
	std::string			_to_parse;
	Request				*_request;

public:
	Client(Socket *listen_sock);
	~Client();

	bool accept_connection();
	void readRequest();

	int get_socket_fd();
	int get_s_addr();
	Request *get_request();
};
