#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "webserv.hpp"

# define BUFFERSIZE 32768

class Serv;
class Request;
class Response;

class Client
{
public:
	Client(Serv *server, Sock *listenSock);
	~Client();
	Serv *getServer() const;
	int getClientfd() const;
	int getStatus() const;
	void setStatus(int status);
	std::string const &getMessage() const;
	Request *getRequest() const;
	Response *getResponse() const;
	int getDomain() const;
	int getPort() const;
	int getAddress() const;
	void acceptConnection();
	void readFromClient();
	void executeRequest();
	void writeToClient();
	void setWaitingTime(long time);
	long getWaitingTime() const;

	enum status
	{
		START,
		ALIVE,
		WRITE,
		DONE,
		DEAD
	};
private:
	Serv *_server;
	Sock *_listenSock;
	int _clientfd;
	int _domain;
	int _port;
	int _address;
	std::string _message;
	std::string _buffer;
	int _status;
	long _waitingTime;
	Response *_response;
	Request *_request;
};

#endif
