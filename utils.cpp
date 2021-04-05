#include "Server.hpp"

// Если процесс пытается записать данные в оборванный сокет при помощи вы­зова write или send,
// то он получит сигнал SIGPIPE, который может быть пере­хвачен соответствующим обработчиком сигнала
// при вызове read или write может возникнуть блокирование:

// закрывается дескриптор файла, открытого только на чтение:
// ядро посылает всем процессам, ожидающим записи в канал, сигнал SIGPIPE
// write -> return (-1);
// errno(EPIPE);
void	signals(void) {
	signal(SIGPIPE, SIG_IGN);
	signal(SIGINT, signal_handler);
}

void	signal_handler(int signal)
{
	if (signal == SIGINT) {
		clear_global_configuration();
		exit(EXIT_SUCCESS);
	}
}

// Shall we check if any of servers have been initiated?
void init_servers()
{
    for (size_t i = 0; i < g_config.server.size(); i++) // TODO: error checking!
    {
		//primary port is the first port in list by default
        unsigned short port = g_config.server[i]->port.front();
        std::string host = g_config.server[i]->host;
        g_config.server[i]->serv_socket = new Socket(port, host);
        g_config.server[i]->serv_socket->to_bind();
        g_config.server[i]->serv_socket->to_listen(SOMAXCONN); // Max length for listen (?)
    }
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
