/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mskinner <v.golskiy@ya.ru>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/31 17:37:21 by mskinner          #+#    #+#             */
/*   Updated: 2021/04/05 13:21:05 by mskinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server() {}

Server::~Server() {}

// Shall we check if any of servers have been initiated?
void init_Servers()
{
    for (size_t i = 0; i < g_config.server.size(); i++) // TODO: error checking!
    {
		//primary poert is the first port in list by default
        unsigned short port = g_config.server[i]->port.front();
        std::string host = g_config.server[i]->host;
        g_config.server[i]->serv_socket = new Socket(port, host);
        g_config.server[i]->serv_socket->to_bind();
        g_config.server[i]->serv_socket->to_listen(SOMAXCONN); // Max length for listen (?)
    }
}

// int select(int nfds, fd_set *restrict readfds,
     // 	fd_set *restrict writefds, fd_set *restrict exceptfds,
	 //   	struct timeval *restrict timeout); -> pass null to either if not interested

//FD_ZERO()
// This macro clears (removes all file descriptors from) set.
// It should be employed as the first step in initializing a
// file descriptor set.

// FD_SET()
// This macro adds the file descriptor fd to set.  Adding a
// file descriptor that is already present in the set is a
// no-op, and does not produce an error -> check if present by FD_ISSET()

// FD_ISSET()
// select() modifies the contents of the sets according to
// the rules described below.  After calling select(), the
// FD_ISSET() macro can be used to test if a file descriptor
// is still present in a set.  FD_ISSET() returns nonzero if
// the file descriptor fd is present in set, and zero if it
// is not.

//Set socket fds for every server
void	set_fds(fd_set &read_fd_sets, fd_set &write_fd_sets) {
	for (size_t i = 0; i < g_config.server.size(); i++) {
		if (!(FD_ISSET(g_config.server[i]->serv_socket->get_fd(), &read_fd_sets)))
			FD_SET(g_config.server[i]->serv_socket->get_fd(), &read_fd_sets);
	}
}

// struct timeval *restrict timeout - specifies the interval that select() should block
// waiting for a file descriptor to become ready - SIGNAL HANDLER?????

int select_loop() {
	fd_set	        read_fd_sets;
	fd_set	        write_fd_sets;
	struct timeval  timeout; // Subject: a request to your server should never hang forever -> set timeout
    int             to_select;

	//1) Timeout implementation: https://stackoverflow.com/questions/9847441/setting-socket-timeout
	timeout.tv_sec = 10;
	timeout.tv_usec = 0;
    int is_Running = 1;
	//Ignored. The nfds parameter is included only for compatibility with Berkeley sockets.
	//Setting it to max size of fds = 1024
	int nfds = FD_SETSIZE;
	// TODO: разобраться с select

	while (is_Running)
	{
		FD_ZERO(&read_fd_sets);
		FD_ZERO(&write_fd_sets);

    	// setting fds: adding readFD and writeFD for each server
		set_fds(read_fd_sets, write_fd_sets);

    	// call select
    	to_select = select(nfds, &read_fd_sets, &write_fd_sets, 0, &timeout);
    	if (!to_select)
    		exit_error_msg("Timeout of session");// timeout
		// error occured -> errno is set to indicate an error
    	else if (to_select == -1)
			exit_error(errno);
        else {
            // delete timeout servers ?
            // add new clients (?)
            // interact with clients
        }
	}
	return EXIT_SUCCESS;
}
