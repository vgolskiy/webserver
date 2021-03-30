#include "signal_handler.hpp"

void sigintHandler(int sig)
{
	(void)sig;
	std::cout << std::endl;
	deleteServersShowMessageAndExit(g_servers, SIGINTEXIT);
}

void setSignals()
{
	signal(SIGPIPE, SIG_IGN);
	signal(SIGINT, sigintHandler);
}
