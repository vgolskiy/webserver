#include "Config.hpp"

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

// struct timeval *restrict timeout - specifies the interval that select() should block
// waiting for a file descriptor to become ready - SIGNAL HANDLER?????

extern t_config						g_config;

# include "Socket.hpp"

// закрывается дескриптор файла, открытого только на чтение:
// ядро посылает всем процессам, ожидающим записи в канал, сигнал SIGPIPE
// write -> return (-1);
// errno(EPIPE);

// Если процесс пытается записать данные в оборванный сокет при помощи вы­зова write или send,
// то он получит сигнал SIGPIPE, который может быть пере­хвачен соответствующим обработчиком сигнала
void	signals(void) {
	signal(SIGPIPE, SIG_IGN);
	signal(SIGINT, signal_handler);
};

void	signal_handler(int signal)
{
	if (signal == SIGINT) {
		clear_global_configuration();
		exit(EXIT_SUCCESS);
	}
}

// при вызове read или write может возникнуть блокирование:

// Shall we check if any of servers have been initiated?
void init_Servers()
{
    for (size_t i = 0; i < g_config.server.size(); i++) // TODO: error checking!
    {
        unsigned short port = g_config.server[i]->port.front(); // retrieve first port, is it correct?
        std::string host = g_config.server[i]->host;
        g_config.server[i]->serv_socket = new Socket(port, host);
        g_config.server[i]->serv_socket->to_bind();
        g_config.server[i]->serv_socket->to_listen(SOMAXCONN); // Max length for listen (?)
    }
}

int select_loop()
{
	fd_set	        read_fd_sets;
	fd_set	        write_fd_sets;
	struct timeval  timeout; // Subject: a request to your server should never hang forever -> set timeout
    int             to_select;
	
    int is_Running = 1;
	int nfds = FD_SETSIZE; // max size of fds
	// TODO: разобраться с select   

	while (is_Running)
	{
		FD_ZERO(&read_fd_sets);
		FD_ZERO(&write_fd_sets);

        // set fds:
		// add readFD and writeFD for each server

        // call select
        to_select = select(nfds, &read_fd_sets, &write_fd_sets, 0, &timeout);
        if (to_select == 0)
        {
            // timeout
            std::cout << "Timeout of session!\n";
            exit(EXIT_SUCCESS);
        }
        else if (to_select == -1)
        {
            // error occured -> errno is set to indicate an error
            // if (signals are caught -> continue ;)
            // esle
            std::cerr << "Error occured!\n";
            exit(EXIT_FAILURE);
        }
        else
        {
            // delete timeout servers ?
            // add new clients (?)
            // interact with clients
        }
	}
	return EXIT_SUCCESS;
}

void	exit_error(int err) {
	error_message(strerror(err));
	clear_global_configuration();
	exit(EXIT_FAILURE);
}

void	exit_error_msg(std::string msg) {
	error_message(msg);
	clear_global_configuration();
	exit(EXIT_FAILURE);
}
