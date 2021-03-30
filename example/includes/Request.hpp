#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "webserv.hpp"

# define TMPBUF "tempbuffer"

class Client;

class Request
{
public:
	Request(Client *client);
	~Request();
	Client *getClient() const;
	std::string const &getMethod() const;
	std::string const &getPath() const;
	std::string const &getQueryString() const;
	std::string const &getProtocol() const;
	std::map<std::string, std::string> &getHeaders();
	std::string const &getBody() const;
	int getStatus() const;
	int getContentLength() const;
	int getContentRemain() const;
	void cutContentRemain(int value);
	void setStatus(int status);
	void parseRequest(std::string &msg);
	void parseRequestNone(std::string &msg, std::vector<std::string> &lines);
	bool parseRequestMethod(std::string &msg, std::vector<std::string> &lines);
	bool parseRequestHeader(std::string &msg, std::vector<std::string> &lines);
	bool parseRequestChunkSize(std::string &msg);
	bool parseRequestChunkBody(std::string &msg);
	void setBadRequest();
	std::pair<std::string, std::string> parseHeaderPair(std::string line);
	std::vector<std::string> getCgiEnvVars(std::string const &locationPath, bool php, char const *script_path);
	void doCGI(std::string const &locationPath);
	void parseCGI();
	bool endOfChunks(std::string const &msg);
	void printRequest() const;

	enum status
	{
		NONE,
		BADREQ,
		METHOD,
		HEADER,
		CHUNKSIZE,
		CHUNKBODY,
		BODY,
		DONE
	};

private:
	Client *_client;
	std::string _method;
	std::string _path;
	std::string _queryString;
	std::string _protocol;
	std::map<std::string, std::string> _headers;
	std::string _body;
	int _status;
	int _line;
	int _contentLength;
	int _contentRemain;
	int _tmp_fd;
};

#endif
