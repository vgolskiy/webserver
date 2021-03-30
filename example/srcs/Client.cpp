#include "Client.hpp"

Client::Client(Serv *server, Sock *listenSock):
	_server(server),
	_listenSock(listenSock),
	_clientfd(-1),
	_status(Client::START),
	_waitingTime(get_ms()),
	_response(NULL),
	_request(NULL)
{
}

Client::~Client()
{
	if (this->_clientfd > 2)
		close(this->_clientfd);
	delete this->_request;
	delete this->_response;
}

Serv *Client::getServer() const
{
	return this->_server;
}

int Client::getClientfd() const
{
	return this->_clientfd;
}

int Client::getStatus() const
{
	return this->_status;
}

void Client::setStatus(int status)
{
	this->_status = status;
}

std::string const &Client::getMessage() const
{
	return this->_message;
}

Request *Client::getRequest() const
{
	return this->_request;
}

Response *Client::getResponse() const
{
	return this->_response;
}

int Client::getDomain() const
{
	return this->_domain;
}

int Client::getPort() const
{
	return this->_port;
}

int Client::getAddress() const
{
	return this->_address;
}

void Client::acceptConnection()
{
	struct sockaddr_in addr;
	socklen_t addrSize = (socklen_t)sizeof(addr);
	this->_clientfd = accept(this->_listenSock->getSockfd(), (struct sockaddr *) &addr, &addrSize);
	if (this->_clientfd == -1)
		throw ErrnoException();

	this->_domain = addr.sin_family;
	this->_port = addr.sin_port;
	this->_address = addr.sin_addr.s_addr;

	if (fcntl(this->_clientfd, F_SETFL, O_NONBLOCK) == -1)
		throw ErrnoException();
	this->_status = Client::ALIVE;
}

static void printMsg(std::string const &msg)
{
	for (size_t i = 0; i < msg.size(); ++i)
	{
		if (msg[i] == '\r')
			std::cout << "[CR]";
		else if (msg[i] == '\n')
			std::cout << "[LF]" << std::endl;
		else
			std::cout << msg[i];
	}
}

void Client::readFromClient()
{
	if (this->_request == NULL)
		this->_request = new Request(this);
	int readSize = BUFFERSIZE;
	int remainContentLen = this->_request->getContentRemain();
	if (this->_request->getStatus() == Request::BODY && remainContentLen > 0)
	{
		if (remainContentLen < BUFFERSIZE)
		{
			readSize = remainContentLen;
		}
	}
	char buf[readSize + 1];
	int ret = recv(this->_clientfd, &buf, readSize, 0);
	if (ret > 0)
	{
		this->_waitingTime = get_ms();
		buf[ret] = 0;
		this->_buffer += buf;
		if (this->_request->getStatus() == Request::BODY)
		{
			this->_request->cutContentRemain(ret);
		}
		this->_request->parseRequest(this->_buffer);
	}
	else if (ret == 0)
	{
		this->_status = Client::DEAD;
	}
	else if (ret == -1)
	{
		this->_request->parseRequest(this->_buffer);
	}
	
	if (g_verbose)
	{
		if (this->_buffer.size() < 1000)
		{
			std::cout << std::endl << "__________ BUFFER ___________" << std::endl; 
			printMsg(this->_buffer);
			std::cout << std::endl << "_____________________________" << std::endl << std::endl;
		}
		std::cout << "\x1B[2J\x1B[H" << std::endl;
		if (this->_request)
			this->_request->printRequest();
	}
	return;
}

void Client::executeRequest()
{
	std::string buf;
	if (this->_request == NULL)
		return;
	if (this->_request->getStatus() == Request::BADREQ)
	{
		if (this->_status == Client::ALIVE)
		{
			this->_response = new Response(this);
			this->_response->setStatusCode(BADREQUEST);
			this->_response->writeResponse();
			this->_status = Client::WRITE;
		}
		this->writeToClient();
	}
	if (this->_request->getStatus() == Request::DONE)
	{
		if (this->_status == Client::ALIVE)
		{
			this->_response = new Response(this);
			this->_response->makeResponse();
			this->_status = Client::WRITE;
		}
		this->writeToClient();
	}
	if (this->_status == Client::DONE)
	{
		delete this->_request;
		delete this->_response;
		this->_request = NULL;
		this->_response = NULL;
		this->_buffer = "";
		this->_status = Client::ALIVE;
	}
}

void Client::writeToClient()
{
	if (this->_status != Client::WRITE)
		return;

	int ret;
	std::string &responseStr = this->_response->getResponse();
	int responseLen = responseStr.size();

	ret = send(this->_clientfd, responseStr.c_str(), responseLen, 0);
	if (ret < 0)
	{
		this->_status = Client::DEAD;
	}
	else
	{
		this->_waitingTime = get_ms();
		if (responseLen <= ret)
		{
			this->_status = Client::DONE;
		}
		else
		{
			responseStr.erase(0, ret);
		}
	}
}

void Client::setWaitingTime(long time)
{
	this->_waitingTime = time;
}

long Client::getWaitingTime() const
{
	return this->_waitingTime;
}
