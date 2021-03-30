#include "webserv.hpp"

bool g_verbose;
std::list<Serv*> g_servers;

static int mainLoop(std::list<Serv*> &servers)
{
	fd_set readFdSet;
	fd_set writeFdSet;
	int fdMax;
	int res;
	struct timeval timeout = {600, 0};

	while (1)
	{
		fdMax = 0;
		FD_ZERO(&readFdSet);
		FD_ZERO(&writeFdSet);
		setFdSets(servers, readFdSet, writeFdSet, fdMax);
		try
		{
			if ((res = select(fdMax + 1, &readFdSet, &writeFdSet, 0, &timeout)) < 1)
				throw ErrnoException();
		}
		catch (std::exception &e)
		{
			if (errno == EINTR)
			{
				errno = 0;
				continue;
			}
			else if (res == 0)
				deleteServersShowMessageAndExit(servers, TIMEOUT);
			else
				deleteServersShowErrorAndExit(servers, e.what());
		}
		deleteTimeoutServersAndClients(servers, 300, 60);
		addNewClients(servers, readFdSet);
		interactWithClients(servers, readFdSet, writeFdSet);
	}
	return EXIT_SUCCESS;
}

int main(int ac, char **av)
{
	g_verbose = false;
	std::string config = readFile(ac, av);
	checkBraces(config);
	g_servers = parseConfFile(config);
	std::list<Serv*> &servers = g_servers;
	confValidate(servers);
	if (g_verbose)
		printServers(servers);
	launchServers(servers);
	if (servers.empty() == true)
		showErrorAndExit("No servers were started");
	setSignals();
	return mainLoop(servers);
}
