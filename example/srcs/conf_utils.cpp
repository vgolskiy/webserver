#include "conf_utils.hpp"

std::string readFile(int ac, char **av)
{
	if (ac == 3 && std::string(av[1]) == "-v")
		g_verbose = true;
	else if (ac != 2)
		showErrorAndExit(ARG);

	int fd;
	if ((fd = open(av[g_verbose ? 2 : 1], O_RDONLY)) < 0)
		showErrorAndExit(FILENOTOPEN);

	struct stat fileStat;
	fstat(fd, &fileStat);
	if (fileStat.st_size == 0)
	{
		close(fd);
		showErrorAndExit(FILEISEMPTY);
	}
	return fileToStr(av[g_verbose ? 2 : 1]);
}

void checkBraces(std::string const &confStr)
{
	int brace = 0;
	std::string::const_iterator it = confStr.begin();
	std::string::const_iterator ite = confStr.end();
	for (; it != ite; ++it)
	{
		if (*it == '{')
			brace++;
		if (*it == '}')
			brace--;
	}
	if (brace != 0)
		showErrorAndExit(CONFNOTVALID);
}

void confValidate(std::list<Serv*> &servers)
{
	std::list<Serv*>::const_iterator i = servers.begin();
	std::list<Serv*>::const_iterator ie = servers.end();
	std::list<Serv*>::const_iterator j;
	for (; i != ie; i++)
	{
		if ((*i)->getServerName().empty() == true ||
			(*i)->getListenPort() == -1)
			deleteServersShowErrorAndExit(servers, CONFNOTVALID);
		j = i;
		j++;
		for (; j != ie; j++)
		{
			if ((*i)->getListenPort() == (*j)->getListenPort() ||
				(*i)->getServerName() == (*j)->getServerName())
				deleteServersShowErrorAndExit(servers, CONFNOTVALID);
		}
	}
}

void printServers(std::list<Serv*> &servers)
{
	std::cout << "\x1B[2J\x1B[H";
	std::list<Serv*>::const_iterator it = servers.begin();
	std::list<Serv*>::const_iterator ite = servers.end();
	for (; it != ite; ++it)
	{
		std::cout << std::endl << "_______ SERVER _______" << std::endl;
		(*it)->print();
		std::cout << "_______ ______ _______" << std::endl << std::endl;
	}
}
