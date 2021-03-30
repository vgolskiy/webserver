#ifndef CONF_UTILS_HPP
# define CONF_UTILS_HPP

# include "webserv.hpp"

class Serv;

std::string readFile(int ac, char **av);
void checkBraces(std::string const &confStr);
void confValidate(std::list<Serv*> &servers);
void printServers(std::list<Serv*> &servers);

#endif
