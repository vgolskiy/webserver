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

class Server {
private:

public:
	Server(void);
	~Server(void);
};

void	signals(void);
void	exit_error(int err);
void	exit_error_msg(std::string msg);
void	init_Servers(void);
int		select_loop(void);
void	set_fds(fd_set &read_fd_sets, fd_set &write_fd_sets);
