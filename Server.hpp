/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mskinner <v.golskiy@ya.ru>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/31 17:35:07 by mskinner          #+#    #+#             */
/*   Updated: 2021/04/05 15:07:46 by mskinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "Config.hpp"
# include "Socket.hpp"
# include "Client.hpp"

class Client;
class Server {
private:

public:
	Server(void);
	~Server(void);

};

void	exit_error(int err);
void	exit_error_msg(std::string msg);
void	signal_handler(int signal);
void	signals(void);
