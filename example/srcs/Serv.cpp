#include "Serv.hpp"

Serv::Serv():
	_status(Serv::START),
	_listenPort(-1),
	_socket(NULL),
	_waitingTime(get_ms())
{
}

Serv::~Serv()
{
	if (this->_clients.empty() == false)
	{
		std::list<Client*>::iterator itClient = this->_clients.begin();
		std::list<Client*>::iterator iteClient = this->_clients.end();
		for (; itClient != iteClient; itClient++)
			delete *itClient;
		this->_clients.clear();
	}
	delete this->_socket;
	if (this->_status == Serv::ALIVE)
		std::cout << "Server [" << this->_serverName << "] stopped." << std::endl;
}

int Serv::getStatus() const
{
	return this->_status;
}

void Serv::setStatus(int status)
{
	this->_status = status;
}

void Serv::addSocket(int domain, int type, int protocol)
{
	this->_socket = new Sock(domain, type, protocol);
	this->_socket->bindSocket(this->_listenPort, INADDR_ANY);
	this->_socket->listenTo(SOMAXCONN);
	if (this->_status == Serv::START)
		std::cout << "Server [" << this->_serverName << "] is listening to port " << this->_listenPort << "." << std::endl;
	this->_status = Serv::ALIVE;
}

Sock *Serv::getSocket() const
{
	return this->_socket;
}

void Serv::addClient()
{
	this->_clients.push_back(new Client(this, this->_socket));
	Client *newClient = this->_clients.back();
	try
	{
		newClient->acceptConnection();
		std::cout << "Server [" << this->_serverName << "]: new client connected." << std::endl;
	}
	catch (std::exception &e)
	{
		this->_clients.pop_back();
		std::cout << "Server [" << this->_serverName << "]: failed to establish connection with new client (" << e.what() << ")." << std::endl;
	}
}

std::list<Client*> const &Serv::getClients() const
{
	return this->_clients;
}

void Serv::setListenPort(int listenPort)
{
	this->_listenPort = listenPort;
}

void Serv::setServerName(std::string serverName)
{
	this->_serverName = serverName;
}

void Serv::setErrorPath(std::string errorPath)
{
	this->_errorPath = errorPath;
}

void Serv::addLocation(std::pair<std::string, t_location> location)
{
	this->_locations.insert(location);
}


int Serv::getListenPort() const
{
	return this->_listenPort;
}

std::string const &Serv::getServerName() const
{
	return this->_serverName;
}

std::string const &Serv::getErrorPath() const
{
	return this->_errorPath;
}

std::map<std::string, t_location> const &Serv::getLocations() const
{
	return this->_locations;
}

void Serv::addReadFdToSet(fd_set &readFdSet, int &fdMax) const
{
	if (FD_ISSET(this->_socket->getSockfd(), &readFdSet) == 0)
	{
		if (this->_socket->getSockfd() > fdMax)
			fdMax = this->_socket->getSockfd();
		FD_SET(this->_socket->getSockfd(), &readFdSet);
	}

	std::list<Client*>::const_iterator itClient = this->_clients.begin();
	std::list<Client*>::const_iterator iteClient = this->_clients.end();
	for (; itClient != iteClient; ++itClient)
	{
		if (FD_ISSET((*itClient)->getClientfd(), &readFdSet) == 0)
		{
			if ((*itClient)->getClientfd() > fdMax)
				fdMax = (*itClient)->getClientfd();
			FD_SET((*itClient)->getClientfd(), &readFdSet);
		}
	}
}

void Serv::addWriteFdToSet(fd_set &writeFdSet, int &fdMax) const
{
	std::list<Client*>::const_iterator itClient = this->_clients.begin();
	std::list<Client*>::const_iterator iteClient = this->_clients.end();
	for (; itClient != iteClient; ++itClient)
	{
		if ((*itClient)->getRequest() != NULL &&
			((*itClient)->getRequest()->getStatus() == Request::DONE ||
			(*itClient)->getRequest()->getStatus() == Request::BADREQ))
		{
			if (FD_ISSET((*itClient)->getClientfd(), &writeFdSet) == 0)
			{
				if ((*itClient)->getClientfd() > fdMax)
					fdMax = (*itClient)->getClientfd();
				FD_SET((*itClient)->getClientfd(), &writeFdSet);
			}
		}
	}
}

void Serv::interactWithClients(fd_set const &readFdSet, fd_set const &writeFdSet)
{
	std::list<Client*>::iterator it = this->_clients.begin();
	std::list<Client*>::iterator ite = this->_clients.end();
	int clientStatus;
	for (; it != ite; ++it)
	{
		clientStatus = (*it)->getStatus();
		if (FD_ISSET((*it)->getClientfd(), &readFdSet))
		{
			this->_waitingTime = get_ms();
			(*it)->readFromClient();
			if (clientStatus == Client::DEAD)
			{
				delete *it;
				it = this->_clients.erase(it);
				std::cout << "Server [" << this->_serverName << "]: client disconnected." << std::endl;
				return;
			}
			if ((*it)->getRequest() && (*it)->getRequest()->getStatus() == Request::DONE)
				std::cout << "Server [" << this->_serverName << "]: received request from a client." << std::endl;
			else if ((*it)->getRequest() && (*it)->getRequest()->getStatus() == Request::BADREQ)
				std::cout << "Server [" << this->_serverName << "]: received bad request from a client." << std::endl;
		}
		if (FD_ISSET((*it)->getClientfd(), &writeFdSet))
		{
			this->_waitingTime = get_ms();
			if (clientStatus == Client::ALIVE)
				std::cout << "Server [" << this->_serverName << "]: sending response to a client." << std::endl;
			(*it)->executeRequest();
		}
	}
}

long Serv::getWaitingTime() const
{
	return this->_waitingTime;
}

void Serv::deleteTimeoutClients(long timeout)
{
	std::list<Client*>::iterator it = this->_clients.begin();
	std::list<Client*>::iterator ite = this->_clients.end();
	for (; it != ite; ++it)
	{
		if ((*it)->getStatus() == Client::ALIVE &&
			get_ms() - (*it)->getWaitingTime() > timeout)
		{
			delete *it;
			it = this->_clients.erase(it);
			std::cout << "Server [" << this->_serverName << "]: client disconnected." << std::endl;
		}
	}
}

void Serv::print() const
{
	std::map<std::string, t_location> _locations;
	std::cout << "Serv name: " << this->_serverName << std::endl;
	std::cout << "Listen port: " << this->_listenPort << std::endl;
	std::cout << "Error path: " << this->_errorPath << std::endl;

	if (this->_locations.size() > 0)
	{
		std::cout << "Locations:" << std::endl;
		std::map<std::string, t_location>::const_iterator it = this->_locations.begin();
		std::map<std::string, t_location>::const_iterator ite = this->_locations.end();
		for (; it != ite; ++it)
		{
			std::cout << ">" << it->first << "<";
			if (it->second.methods.empty() == false)
			{
				std::cout << "\tmethods: ";
				for (size_t i = 0; i < it->second.methods.size(); i++)
					std::cout << it->second.methods[i] << " ";
			}
			std::cout << std::endl;
			if (it->second.root.empty() == false)
				std::cout << "\troot: " << it->second.root << std::endl;
			if (it->second.index.empty() == false)
				std::cout << "\tindex: " << it->second.index << std::endl;
			std::cout << "\tautoindex: " << (it->second.autoindex ? "on" : "off") << std::endl;
			if (it->second.CGI.empty() == false)
				std::cout << "\tCGI: " << it->second.CGI << std::endl;
			if (it->second.exec.empty() == false)
				std::cout << "\texec: " << it->second.exec << std::endl;
			if (it->second.php.empty() == false)
				std::cout << "\tphp: " << it->second.php << std::endl;
			if (!it->second.auth.first.empty() && !it->second.auth.second.empty())
				std::cout << "\tauth: " << it->second.auth.first << ":" << it->second.auth.second << std::endl;
			if (it->second.max_body != -1)
				std::cout << "\tmax_body: " << it->second.max_body << std::endl;
		}
	}
}
