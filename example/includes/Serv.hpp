#ifndef SERV_HPP
# define SERV_HPP

# include "webserv.hpp"

class Sock;
class Client;
class Response;

typedef struct	s_location
{
	std::vector<std::string> methods;
	std::string root;
	std::string index;
	bool autoindex;
	std::string CGI;
	std::string exec;
	std::string php;
	std::pair<std::string, std::string> auth;
	int	max_body;
}				t_location;

class Serv
{
public:
	Serv();
	~Serv();
	int getStatus() const;
	void setStatus(int status);
	void addSocket(int domain, int type, int protocol);
	Sock *getSocket() const;
	void addClient();
	std::list<Client*> const &getClients() const;
	void setListenPort(int listenPort);
	void setServerName(std::string serverName);
	void setErrorPath(std::string errorPath);
	void addLocation(std::pair<std::string, t_location> location);
	int getListenPort() const;
	std::string const &getServerName() const;
	std::string const &getErrorPath() const;
	std::map<std::string, t_location> const &getLocations() const;
	void addReadFdToSet(fd_set &readFdSet, int &fdMax) const;
	void addWriteFdToSet(fd_set &writeFdSet, int &fdMax) const;
	void interactWithClients(fd_set const &readFdSet, fd_set const &writeFdSet);
	long getWaitingTime() const;
	void deleteTimeoutClients(long timeout);
	void print() const;

	enum status
	{
		START,
		ALIVE,
		DEAD
	};
private:
	int _status;
	int _listenPort;
	std::string _serverName;
	std::string _errorPath;
	std::map<std::string, t_location> _locations;
	Sock *_socket;
	std::list<Client*> _clients;
	long _waitingTime;
};

#endif
