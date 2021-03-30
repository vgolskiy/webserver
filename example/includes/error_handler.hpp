#ifndef ERROR_HANDLER_HPP
# define ERROR_HANDLER_HPP

# define FILENOTOPEN	"Can't open file."
# define CONFNOTVALID	"Not a valid configuration file."
# define ARG			"Not valid arguments."
# define FILEISEMPTY	"Empty file."
# define ADDSOCKET		"Socket adding failed."
# define TIMEOUT		"Client actions timeout."
# define SIGINTEXIT		"Bye-bye."

# include "webserv.hpp"

class Serv;

void showMessageAndExit(std::string const &message);
void showErrorAndExit(std::string const &errorMessage);
void deleteServersShowErrorAndExit(std::list<Serv*> &servers, std::string const &errorMessage);
void deleteServersShowMessageAndExit(std::list<Serv*> &servers, std::string const &errorMessage);

#endif
