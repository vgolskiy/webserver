#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "webserv.hpp"

# define OK 			"200 OK"
# define CREATED		"201 Created"
# define BADREQUEST		"400 Bad Request"
# define UNAUTHORIZED	"401 Unauthorized"
# define NOTFOUND 		"404 Not Found"
# define NOTALLOWED		"405 Method Not Allowed"
# define REQTOOLARGE	"413 Payload Too Large"
# define INTERNALERROR	"500 Internal Server Error"
# define NOTIMPLEMENTED	"501 Not Implemented"

class Client;

class Response
{
public:
	Response(Client *client);
	~Response();
	Client *getClient() const;
	std::string const &getStatusCode() const;
	void setStatusCode(std::string const &code);
	std::string const &getLocation() const;
	std::string const &getFullPath() const;
	std::string const &getFileName() const;
	std::map<std::string, std::string> const &getHeaders() const;
	std::string const &getBody() const;
	void setDate();
	std::string const &getDate() const;
	std::string giveResponse(Request *req, int client_fd);
	void makeResponse();
	void setResponse(std::string const &msg);
	std::string &getResponse();
	bool checkLocation();
	void readBody(std::string &path);
	void writeResponse();
	void checkBody();
	bool checkFile();
	void createIndex();
	void changeHeaders();
private:
	Client *_client;
	std::string _location;
	std::string _fullpath;
	std::string _filename;
	std::string _statusCode;
	std::map<std::string, std::string> _headers;
	std::string _body;
	std::string _date;
	std::string _response;
};

#endif
