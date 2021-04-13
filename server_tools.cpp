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

/*
int nfds:
Представляет собой целое число, на единицу большее максимального файлового дескриптора 
в любом из наборов. Другими словами, при добавлении файловых дескрипторов в наборы вам 
необходимо вычислять максимальное целое значение любого из них, а затем увеличивать
это значение на единицу и передавать в nfds.
*/

# include "Client.hpp"
# include "Config.hpp"

void add_client(t_server* server)
{
    Client *new_cl = new Client(server->serv_socket);
    server->_num_clients.push_back(new_cl);

	if ((server->_num_clients.back()->accept_connection()) == false)
	{
		server->_num_clients.pop_back();
		std::cerr << "Failed to establish connection with a client!\n";
	}
}

void add_new_client(fd_set &read_fd_sets)
{
    for (size_t i = 0; i < g_config.server.size(); i++)
    {
        if (FD_ISSET(g_config.server[i]->serv_socket->get_fd(), &read_fd_sets))
            add_client(g_config.server[i]);
    }
}

// Set socket fds for every server: read_fd_set for servers and both write_and_read_fds for clients
void	set_fds(fd_set &read_fd_sets, fd_set &write_fd_sets, int &nfds)
{
	(void)write_fd_sets;
	for (size_t i = 0; i < g_config.server.size(); i++)
	{
		if (!(FD_ISSET(g_config.server[i]->serv_socket->get_fd(), &read_fd_sets)))
		{
			FD_SET(g_config.server[i]->serv_socket->get_fd(), &read_fd_sets);
			if (g_config.server[i]->serv_socket->get_fd() > nfds)
				nfds = g_config.server[i]->serv_socket->get_fd();
		}
	}
	for (size_t j = 0; j < g_config.server.size(); j++)
	{
		std::list<Client*>::iterator it = g_config.server[j]->_num_clients.begin();
		std::list<Client*>::iterator ite = g_config.server[j]->_num_clients.end();
		for (; it != ite; it++)
		{	
			if (!(FD_ISSET((*it)->get_socket_fd(), &read_fd_sets)))
			{
				FD_SET((*it)->get_socket_fd(), &read_fd_sets);
				if ((*it)->get_socket_fd() > nfds)
					nfds = (*it)->get_socket_fd();
			}
			if ((*it)->get_request()) // TODO: add conditions
			{
				if (!(FD_ISSET((*it)->get_socket_fd(), &write_fd_sets)))
				{
					FD_SET((*it)->get_socket_fd(), &write_fd_sets);
					if ((*it)->get_socket_fd() > nfds)
						nfds = (*it)->get_socket_fd();
				}
			}
		}
	}
}

void deal_request(fd_set &read_fd_sets, fd_set &write_fd_sets) // doesnt work appropriately
{
	(void) write_fd_sets;
	(void) read_fd_sets;

	for (size_t i = 0; i != g_config.server.size(); i++)
	{
		std::list<Client*>::iterator it = g_config.server[i]->_num_clients.begin();
		std::list<Client*>::iterator ite = g_config.server[i]->_num_clients.end();

		for (; it != ite; it++)
			(*it)->readRequest();
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
	int nfds;

	while (is_Running)
	{
		nfds = 0;
		FD_ZERO(&read_fd_sets);
		FD_ZERO(&write_fd_sets);

    	// setting fds: adding readFD and writeFD for each server
		set_fds(read_fd_sets, write_fd_sets, nfds);

    	// call select
    	to_select = select(nfds + 1, &read_fd_sets, &write_fd_sets, 0, &timeout);
    	if (!to_select)
			error_message("Timeout of session");
		// error occured -> errno is set to indicate an error
    	else if (to_select == -1)
			exit_error(errno);
        else {
            // delete timeout servers ?
            add_new_client(read_fd_sets);
            // interact with clients (read_fd_sets + write_fd_sets)
			deal_request(read_fd_sets, write_fd_sets);
        }
	}
	return EXIT_SUCCESS;
}