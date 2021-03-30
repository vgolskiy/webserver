#ifndef WEBSERV_UTILS_HPP
# define WEBSERV_UTILS_HPP

# include "webserv.hpp"

class Serv;

void launchServers(std::list<Serv*> &servers);
void setFdSets(std::list<Serv*> const &servers, fd_set &readFdSet, fd_set &writeFdSet, int &fdMax);
void addNewClients(std::list<Serv*> &servers, fd_set const &readFdSet);
void interactWithClients(std::list<Serv*> &servers, fd_set const &readFdSet, fd_set const &writeFdSet);
void deleteTimeoutServersAndClients(std::list<Serv*> &servers, long timeoutServer, long timeoutClient);
long get_ms();
void deleteServers(std::list<Serv*> &servers);
std::string fileToStr(char const filename[]);
void strToFd(std::string const &str, int fd);
std::string base64encode(std::string const &decoded);
std::string base64decode(std::string encoded);
std::string ntoaddr(int n);

#endif
