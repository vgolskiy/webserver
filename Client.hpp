/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mskinner <v.golskiy@ya.ru>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/31 17:35:07 by mskinner          #+#    #+#             */
/*   Updated: 2021/04/20 14:20:23 by mskinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

# include "Config.hpp"
# include "Request.hpp"

class Config;
class Socket;
class Request;
class Client
{
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
	long				_time_start;

public:
	/*
	Statuses:
		INIT - in a constructor;
		ALIVE - once connection accepted
		DONE - once response sent and nothing left (send returns >= 0) + TODO: delete request+response
		DEAD - if recv returns 0 or if send returns (<0)
	*/

	enum status {
		INIT,
		ALIVE,
		DONE,
		EMPTY
	};

	Client(Socket *listen_sock);
	~Client();

	void	accept_connection(void);
	void	read_run_request(const int i);

	int		get_fd(void);
	int		get_s_addr(void);
	Request	*get_request(void);
	void	clear_request(void);
	int		get_status(void) const;
	long	get_start_time(void) const;
};

#endif
