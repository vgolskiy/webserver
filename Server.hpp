/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mskinner <v.golskiy@ya.ru>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/31 17:35:07 by mskinner          #+#    #+#             */
/*   Updated: 2021/04/05 13:16:37 by mskinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "Socket.hpp"
# include "Client.hpp"

extern t_config						g_config;

class Client;
class Server {
private:

public:
	Server(void);
	~Server(void);

};

void	exit_error(int err);
void	exit_error_msg(std::string msg);
