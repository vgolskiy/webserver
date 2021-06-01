/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mskinner <v.golskiy@ya.ru>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/15 00:11:13 by mskinner          #+#    #+#             */
/*   Updated: 2021/06/01 21:46:21 by mskinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "Config.hpp"

int		start_servers(std::vector<t_server*> &servers);
int 	select_loop(std::vector<t_server*> &servers);
void	add_new_client(std::vector<t_server*> &servers, fd_set &read_fd_sets);
void	set_server_fds(std::vector<t_server*> &servers, fd_set &read_fd_sets, int &nfds);
void	set_client_fds(std::vector<t_server*> &servers, fd_set &read_fd_sets,
				fd_set &write_fd_sets, int &nfds);
void	delete_clients(std::vector<t_server*> &servers);
void	deal_request(std::vector<t_server*> &servers,
					fd_set &read_fd_sets, fd_set &write_fd_sets);
void	send_response(Client *client, t_server *server);

#endif
