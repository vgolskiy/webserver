#include "Request.hpp"

Request::Request(Client *client):
	_client(client),
	_body(""),
	_status(Request::NONE),
	_line(0),
	_contentLength(-1),
	_contentRemain(0),
	_tmp_fd(-1)
{
}

Request::~Request() {}

Client *Request::getClient() const
{
	return this->_client;
}

std::string const &Request::getMethod() const
{
	return this->_method;
}

std::string const &Request::getPath() const
{
	return this->_path;
}

std::string const &Request::getQueryString() const
{
	return this->_queryString;
}

std::string const &Request::getProtocol() const
{
	return this->_protocol;
}

std::map<std::string, std::string> &Request::getHeaders()
{
	return this->_headers;
}

std::string const &Request::getBody() const
{
	return this->_body;
}

int Request::getStatus() const
{
	return this->_status;
}

int Request::getContentLength() const
{
	return this->_contentLength;
}

int Request::getContentRemain() const
{
	return this->_contentRemain;
}

void Request::cutContentRemain(int value)
{
	this->_contentRemain -= value;
}

static std::vector<std::string> messageSplit(std::string msg, std::string const &delimeter)
{
	std::vector<std::string> split;

	size_t pos = 0;
	while (msg.size() > 0)
	{
		pos = msg.find(delimeter);
		if (pos == std::string::npos)
		{
			split.push_back(msg);
			break;
		}
		split.push_back(msg.substr(0, pos));
		msg.erase(0, pos + delimeter.size());
		if (msg.size() == 0)
			split.push_back("");
	}
	return split;
}

void Request::printRequest() const 
{
	if (this->_method.empty() == true && this->_path.empty() == true && this->_protocol.empty() == true && \
		this->_headers.empty() == true && this->_body.empty() == true)
	{
		std::cout << "Bad message (all fields are empty)" << std::endl;
		return;
	}
	std::cout << "_________ Request ________" << std::endl;
	if (this->_method.empty() == false)
		std::cout << "Method: " << this->_method << std::endl;
	if (this->_path.empty() == false)
		std::cout << "Path: " << this->_path << std::endl;
	if (this->_protocol.empty() == false)
		std::cout << "Protocol: " << this->_protocol << std::endl;
	std::cout << "Content length: " << this->_contentLength << std::endl;
	std::cout << "Content remain: " << this->_contentRemain << std::endl;
	std::cout << "Content sum len: " << this->_body.size() << std::endl;
	if (this->_headers.empty() == false)
	{
		std::cout << "_____ _____ _____" << std::endl;
		std::cout << "Headers:" << std::endl;
		std::map<std::string, std::string>::const_iterator it = this->_headers.begin();
		std::map<std::string, std::string>::const_iterator ite = this->_headers.end();
		for (; it != ite; ++it)
			std::cout << it->first << ": " << it->second << std::endl;
	}
	
	if (this->_body.empty() == false)
	{
		if (this->_body.size() < 5000)
		{
			std::cout << "_____ _____ _____" << std::endl;
			std::cout << std::endl << "Body:" << std::endl << this->_body << std::endl;
		}
		else
		{
			std::cout << "_____ _____ _____" << std::endl;
			std::cout << std::endl << "Body is TOO BIG (> 5000)" << std::endl;
		}

	}
	
	std::cout << "_____ _____ _____" << std::endl;
	std::cout << "status: ";
	switch (this->_status)
	{
		case Request::NONE:
			std::cout << "NONE";
			break;
		case Request::BADREQ:
			std::cout << "BADREQ";
			break;
		case Request::METHOD:
			std::cout << "METHOD";
			break;
		case Request::HEADER:
			std::cout << "HEADER";
			break;
		case Request::BODY:
			std::cout << "BODY";
			break;
		case Request::DONE:
			std::cout << "DONE";
			break;
		case Request::CHUNKBODY:
			std::cout << "CHUNKBODY";
			break;
		case Request::CHUNKSIZE:
			std::cout << "CHUNKSIZE";
			break;
	}
	std::cout << std::endl << "_____ End of request _____" << std::endl << std::endl;
}

void Request::setBadRequest()
{
	this->_status = Request::BADREQ;
}

static bool isValidHeaderField(std::string const &line)
{
	size_t colon_pos = line.find(":");
	if (colon_pos == std::string::npos)
		return false;
	if (colon_pos > line.find(" ") || colon_pos > line.find("	"))
		return false;
	size_t crlf_pos = line.size();
	if (colon_pos == crlf_pos - 1)
		return false;
	return true;
}

std::pair<std::string, std::string> Request::parseHeaderPair(std::string line)
{
	std::string key;

	size_t colon_pos = line.find(":");
	key = line.substr(0, colon_pos);
	if (Http::isValidHeader(key))
		key = Http::getRightCaseHeader(key);

	line.erase(0, colon_pos + 1);
	for (size_t i = 0; i < line.size(); ++i)
	{
		if (line[i] != ' ' && line[i] != '	')
		{
			line.erase(0, i);
			break;
		}
	}
	for (int j = line.size() - 1; j >= 0; --j)
	{
		if (line[j] != ' ' && line[j] != '	')
		{
			line.erase(j + 1, std::string::npos);
			break;
		}
	}
	if (key == "Content-Length")
	{
		try
		{
			this->_contentLength = std::stoi(line);
			this->_contentRemain = this->_contentLength;
		}
		catch (std::exception &e)
		{
			this->_contentLength = -1;
		}
	}
	else if (key == "Transfer-Encoding")
	{
		if (line == "chunked")
		{
			this->_contentLength = -2;
		}
	}

	return std::pair<std::string, std::string>(key, line);
}

static bool isHexNumber(std::string const &hex)
{
	char const hexNums[] = "0123456789abcdefABCDEF";
	int hexNumsLen = sizeof(hexNums);
	int hexLen = hex.size();
	if (hexLen == 0)
		return false;
	for (int i = 0; i < hexLen; ++i)
	{
		for (int j = 0; j < hexNumsLen; ++j)
		{
			if (hex[i] == hexNums[j])
				break;
			if (j == hexNumsLen - 1)
				return false;
		}
	}
	return true;
}

void Request::parseRequest(std::string &msg)
{
	if (this->_status == Request::DONE ||
		this->_status == Request::BADREQ)
		return;
	if (this->_status == Request::NONE ||
		this->_status == Request::METHOD ||
		this->_status == Request::HEADER)
	{
		std::vector<std::string> lines = messageSplit(msg, "\r\n");
		if (this->_status == Request::NONE && this->_line < (int)lines.size())
		{
			this->parseRequestNone(msg, lines);
		}
		if (this->_status != Request::BODY &&
			msg.find("\r\n\r\n") == std::string::npos &&
			msg.rfind("\r\n") != msg.size() - 2)
			return;
		if (this->_status == Request::METHOD && this->_line < (int)lines.size() && lines[this->_line].empty() == false)
		{
			if (parseRequestMethod(msg, lines))
				return this->setBadRequest();
		}
		if (this->_status == Request::HEADER && this->_line < (int)lines.size())
		{
			if (parseRequestHeader(msg, lines))
				return this->setBadRequest();
		}
	}
	if (this->_status == Request::BODY)
	{
		if (this->_contentLength <= (int)msg.size())
		{
			this->_body = msg;
			this->_status = Request::DONE;
		}
	}
	if (this->_status == Request::CHUNKSIZE)
	{
		if (this->parseRequestChunkSize(msg))
			return;
	}
	if (this->_status == Request::CHUNKBODY)
	{
		if (parseRequestChunkBody(msg))
			return;
		else
			return this->parseRequest(msg);
	}
}

void Request::parseRequestNone(std::string &msg, std::vector<std::string> &lines)
{
	while (this->_line < (int)lines.size())
	{
		if (lines[this->_line].empty() == true)
		{
			lines.erase(lines.begin());
			msg.erase(0, msg.find("\r\n") + 2);
		}
		else
		{
			this->_status = Request::METHOD;
			break;
		}
	}
}

bool Request::parseRequestMethod(std::string &msg, std::vector<std::string> &lines)
{
	static_cast<void>(msg);
	std::vector<std::string> methodLine = messageSplit(lines[this->_line], " ");
	if (methodLine.size() != 3)
		return true;
	this->_method = methodLine[0];
	if (Http::isValidMethod(this->_method) == false)
		return true;
	this->_path = methodLine[1];
	if (this->_path.find("?") != std::string::npos)
	{
		if (std::count(this->_path.begin(), this->_path.end(), '?') != 1)
			return true;
		this->_queryString = this->_path.substr(this->_path.find("?") + 1, std::string::npos);
		this->_path.erase(this->_path.find("?"), std::string::npos);
	}
	this->_protocol = methodLine[2];
	if (Http::isValidProtocol(this->_protocol) == false)
		return true;
	this->_line++;
	this->_status = Request::HEADER;

	return false;
}

bool Request::parseRequestHeader(std::string &msg, std::vector<std::string> &lines)
{
	while (this->_line < (int)lines.size() && lines[this->_line].empty() == false)
	{
		if (isValidHeaderField(lines[this->_line]) == false)
			return true;
		this->_headers.insert(this->parseHeaderPair(lines[this->_line]));
		this->_line++;
	}
	if (this->_line < (int)lines.size() && this->_line + 1 < (int)lines.size() && 
		lines[this->_line].empty() == true)
	{
		msg.erase(0, msg.find("\r\n\r\n") + 4);
		if (this->_contentLength > 0)
		{
			this->_status = Request::BODY;
			this->_contentRemain -= msg.size();
		}
		else if (this->_contentLength == -2)
		{
			this->_status = Request::CHUNKSIZE;
		}
		else
		{
			this->_status = Request::DONE;
		}
		this->_line++;
	}
	return false;
}

bool Request::parseRequestChunkSize(std::string &msg)
{
	if (msg.find("\r\n") == std::string::npos)
		return true;
	else if (!isHexNumber(msg.substr(0, msg.find("\r\n"))))
	{
		this->_status = Request::BADREQ;
		return true;
	}
	this->_contentLength = std::strtol(msg.c_str(), 0, 16);
	this->_contentRemain = this->_contentLength;		
	msg.erase(0, msg.find("\r\n") + 2);
	this->_status = Request::CHUNKBODY;
	return false;
}

bool Request::parseRequestChunkBody(std::string &msg)
{
	if (this->_contentLength == 0 && msg == "\r\n")
		this->_status = Request::DONE;
	else if (this->_contentLength + 2 <= (int)msg.size())
	{
		this->_body += msg.substr(0, this->_contentLength);
		msg.erase(0, this->_contentLength);
		if (msg.find("\r\n") != 0)
		{
			this->_status = Request::BADREQ;
			return true;
		}
		msg.erase(0, 2);
		this->_status = Request::CHUNKSIZE;
		return false;
	}
	return true;
}

void Request::setStatus(int status)
{
	this->_status = status;
}

std::vector<std::string> Request::getCgiEnvVars(std::string const &locationPath, bool php, char const *script_path)
{
	std::vector<std::string> env;
	std::string tmp;
	std::map<std::string, t_location> locations = this->_client->getServer()->getLocations();
	t_location location = locations[locationPath];

	if (this->_headers.count("Authorization") == 1)
		php ? (void)0 : env.push_back("AUTH_TYPE=Basic");
	env.push_back("CONTENT_LENGTH=" + std::to_string(this->_body.size()));
	if (this->_headers["Content-Type"].empty() == false)
		env.push_back("CONTENT_TYPE=" + this->_headers["Content-Type"]);
	php ? (void)0 : env.push_back("GATEWAY_INTERFACE=CGI/1.1");
	env.push_back("PATH_INFO=" + locationPath);
	env.push_back("PATH_TRANSLATED=" + this->_client->getResponse()->getFullPath());
	if (this->_queryString.empty() == false)
		env.push_back("QUERY_STRING=" + this->_queryString);
	env.push_back("REMOTE_ADDR=" + ntoaddr(this->_client->getAddress()));
	if (location.auth.first.empty() == false)
	{
		env.push_back("REMOTE_IDENT=" + location.auth.first);
		env.push_back("REMOTE_USER=" + location.auth.first);
	}
	php ? (void)0 : env.push_back("REQUEST_METHOD=" + this->_method);
	env.push_back("REQUEST_URI=" + locationPath);
	php && script_path ? env.push_back("SCRIPT_NAME=" + std::string(script_path)) : (void)0;
	php ? (void)0 : env.push_back("SERVER_NAME=" + this->getClient()->getServer()->getServerName());
	env.push_back("SERVER_PORT=" + std::to_string(this->getClient()->getServer()->getListenPort()));
	env.push_back("SERVER_PROTOCOL=" + this->_protocol);
	php ? (void)0 : env.push_back("SERVER_SOFTWARE=webserv");

	std::map<std::string, std::string>::const_iterator it = this->_headers.begin();
	std::map<std::string, std::string>::const_iterator ite = this->_headers.end();
	for (; it != ite; it++)
	{
		if (it->second.empty() == true)
			continue;
		tmp = it->first;
		std::transform(tmp.begin(), tmp.end(), tmp.begin(), ::toupper);
		std::replace(tmp.begin(), tmp.end(), '-', '_');
		env.push_back("HTTP_" + tmp + "=" + it->second);
	}
	return env;
}

void Request::doCGI(std::string const &locationPath)
{
	std::map<std::string, t_location> locations = this->_client->getServer()->getLocations();

	char const *cgi_path;
	char const *script_path;
	bool php = false;
	
	if (this->_path.rfind(".php") == this->_path.size() - 4 &&
		locations[locationPath].php.empty() == false)
	{
		cgi_path = (locations[locationPath].php).c_str();
		size_t pos = this->_path.rfind("/");
		script_path = (this->_path.substr(pos == std::string::npos ? 0 : pos + 1, std::string::npos)).c_str();
		php = true;
	}
	else
	{
		cgi_path = (locations[locationPath].exec).c_str();
		script_path = NULL;
	}
	char const *args[] = {cgi_path, script_path, NULL};
	
	std::vector<std::string> envStr = this->getCgiEnvVars(locationPath, php, script_path);
	std::vector<char const *> envChar;
	std::vector<std::string>::const_iterator itStr = envStr.begin();
	std::vector<std::string>::const_iterator iteStr = envStr.end();
	for (; itStr != iteStr; ++itStr)
		envChar.push_back(itStr->c_str());
	envChar.push_back(NULL);
	
	int fd[2];
	pid_t pid;
	if ((this->_tmp_fd = open(TMPBUF, O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0 ||
		pipe(fd) < 0 ||
		(pid = fork()) < 0)
		throw ErrnoException();
	if (pid == 0)
	{
		close(fd[1]);
		dup2(fd[0], 0);
		close(fd[0]);
		dup2(this->_tmp_fd, 1);

		int exec;
		if (chdir((locations[locationPath].root).c_str()))
			throw ErrnoException();
		if ((exec = execve(cgi_path, (char *const *)args, (char *const *)&envChar[0])) < 0)
			throw ErrnoException();
	}
	else if (pid > 0)
	{
		close(fd[0]);
		strToFd(this->_body, fd[1]);
		int stat;
		waitpid(pid, &stat, 0);
		close(fd[1]);
		close(this->_tmp_fd);
	}
}

void Request::parseCGI()
{
	std::string buf = fileToStr(TMPBUF);
	unlink(TMPBUF);
	std::string header = buf.substr(0, buf.find("\r\n\r\n"));
	std::string code;
	if (header.find("Status: ") != std::string::npos)
	{
		header.erase(0, header.find("Status: "));
		if (header.find("\r\n") != std::string::npos)
			code = header.substr(0, header.find("\r\n"));
		else
			code = header;
		code.erase(0, 8);
		this->_client->getResponse()->setStatusCode(code);
	}
	buf.erase(0, buf.find("\r\n\r\n") + 4);
	this->_body = buf;
}
