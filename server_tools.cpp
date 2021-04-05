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

# include "Socket.hpp"
# include "Client.hpp"

void add_client(t_server* server)
{
    Client *new_cl = new Client(server->serv_socket);
    server->_num_clients.push_back(new_cl);

	// client->accept_connection();
}

void add_new_client(fd_set &read_fd_sets)
{
    for (size_t i = 0; i < g_config.server.size(); i++)
    {
        if (FD_ISSET(g_config.server[i]->serv_socket->get_fd(), &read_fd_sets))
        {
            add_client(g_config.server[i]);
            // add new Client to server (either in class or separate function)
            // accept connection -> if not connected -> "Fail to connect"
            ;
        }
    }
}

//Set socket fds for every server
void	set_fds(fd_set &read_fd_sets, fd_set &write_fd_sets) {
	(void) write_fd_sets;
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

	while (is_Running)
	{
		FD_ZERO(&read_fd_sets);
		FD_ZERO(&write_fd_sets);

    	// setting fds: adding readFD and writeFD for each server
		set_fds(read_fd_sets, write_fd_sets);

    	// call select
    	to_select = select(nfds, &read_fd_sets, &write_fd_sets, 0, &timeout);
    	if (!to_select)
    		exit_error_msg("Timeout of session");
		// error occured -> errno is set to indicate an error
    	else if (to_select == -1)
			exit_error(errno);
        else {
            // delete timeout servers ?
            add_new_client(read_fd_sets);
            // interact with clients (read_fd_sets + write_fd_sets)
        }
	}
	return EXIT_SUCCESS;
}