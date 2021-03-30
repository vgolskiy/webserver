#include "error_handler.hpp"

void showMessageAndExit(std::string const &message)
{
	std::cout << message << std::endl;
	exit(EXIT_SUCCESS);
}

void showErrorAndExit(std::string const &errorMessage)
{
	std::cerr << "Error: " << errorMessage << std::endl;
	exit(EXIT_FAILURE);
}

void deleteServersShowMessageAndExit(std::list<Serv*> &servers, std::string const &errorMessage)
{
	deleteServers(servers);
	showMessageAndExit(errorMessage);
}

void deleteServersShowErrorAndExit(std::list<Serv*> &servers, std::string const &errorMessage)
{
	deleteServers(servers);
	showErrorAndExit(errorMessage);
}
