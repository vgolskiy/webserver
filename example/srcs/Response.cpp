#include "Response.hpp"

Response::Response(Client *client): _client(client) {}

Response::~Response() {}

Client *Response::getClient() const
{
	return this->_client;
}

std::string const &Response::getStatusCode() const
{
	return this->_statusCode;
}

void Response::setStatusCode(std::string const &code)
{
	this->_statusCode = code;
}

std::string const &Response::getLocation() const
{
	return this->_location;
}

std::string const &Response::getFullPath() const
{
	return this->_fullpath;
}

std::string const &Response::getFileName() const
{
	return this->_filename;
}

std::map<std::string, std::string> const &Response::getHeaders() const
{
	return this->_headers;
}

std::string const &Response::getBody() const
{
	return this->_body;
}

std::string const &Response::getDate() const
{
	return this->_date;
}

void Response::setDate()
{
	struct tm tm;
	struct timeval tv;
	char buf[29];
	gettimeofday(&tv, NULL);
	std::string seconds = std::to_string((long long int)tv.tv_sec);
	strptime(seconds.c_str(), "%s", &tm);
	strftime(buf, sizeof(buf), "%a %d %b %Y %T", &tm);
	this->_date = buf;
}

void Response::createIndex()
{
	DIR *dir;
	struct dirent *current;
	std::string body;

	dir = opendir(this->getFullPath().c_str());
	body = "<html>\n<body>\n";
	body += "<h1>Directory listing:</h1>\n";
	while ((current = readdir(dir)) != NULL)
	{
		if (current->d_name[0] != '.')
		{
			body += "<a href=\"" + this->getFileName();
			if (!this->getFileName().empty() && this->getFileName()[this->getFileName().size() - 1] != '/')
				body += "/";
			body += current->d_name;
			body += "\">";
			body += current->d_name;
			body += "</a><br>\n";
		}
	}
	closedir(dir);
	body += "</body>\n</html>\n";
	this->_body = body;
	
}

void Response::checkBody()
{
	struct stat	stat;
	Request *req = this->_client->getRequest();
	std::string errorpath = this->_client->getServer()->getErrorPath();
	if (this->getStatusCode() != OK && req->getMethod() != "HEAD")
	{
		if (!errorpath.empty())
		{
			if (errorpath[errorpath.size() - 1] != '/')
				errorpath.push_back('/');
			errorpath += this->getStatusCode().substr(0, 3);
			errorpath += ".html";
			if (!lstat(errorpath.c_str(), &stat))
			{
				readBody(errorpath);
				return ;
			}
		}
		this->_body = this->getStatusCode();
	}
}

void Response::changeHeaders()
{
	this->_headers["Server"] = "webserv";
	this->setDate();
	this->_headers["Date"] = this->getDate() + " GMT";
	this->_headers["Retry-After"] = "1";
}

void Response::writeResponse()
{
	this->_response = "HTTP/1.1 ";
	this->_response += this->_statusCode;
	this->_response += "\r\n";
	if (this->getBody().size() == 0)
		checkBody();
	changeHeaders();
	std::map<std::string, std::string>::iterator it = this->_headers.begin();
	std::map<std::string, std::string>::iterator ite = this->_headers.end();
	while (it != ite)
	{
		this->_response += (*it).first;
		this->_response += ": ";
		this->_response += (*it).second;
		this->_response += "\r\n";
		it++;
	}
	this->_response += "Content-Length: ";
	this->_response += std::to_string(this->getBody().size());
	this->_response += "\r\n\r\n";
	this->_response += this->getBody();
}

void Response::readBody(std::string &path)
{
	try
	{
		this->_body = fileToStr(path.c_str());
	}
	catch (std::exception &e)
	{
		this->_statusCode = INTERNALERROR;
	}
}

bool Response::checkFile()
{
	struct stat	stat;
	DIR *dir;
	std::map<std::string, t_location> loc = this->_client->getServer()->getLocations();
	std::string root = loc[this->getLocation()].root;
	std::string index = loc[this->getLocation()].index;
	
	if (root.size() > 1 && root[root.size() - 1] != '/' && this->getFileName().size() && this->getFileName()[0] != '/')
		root.push_back('/');
	std::string fullpath = root + this->getFileName();
	if ((dir = opendir((fullpath).c_str())))
	{
		if (fullpath[fullpath.size() - 1] != '/')
			fullpath.push_back('/');
		this->_fullpath = fullpath;
		if (!index.empty() && !lstat((fullpath + index).c_str(), &stat))
		{
			this->_fullpath = fullpath + index;
			if (loc[this->getLocation()].auth.first.empty())
				readBody(this->_fullpath);
		}
		else if (loc[this->getLocation()].autoindex)
			createIndex();
		else
		{
			this->_body = "Sorry, this is a directory";
			closedir(dir);
			return false;
		}
		closedir(dir);
		return true;
	}
	else
	{
		if (!lstat((fullpath).c_str(), &stat))
		{
			this->_fullpath = fullpath;
			if (loc[this->getLocation()].auth.first.empty())
				readBody(fullpath);
			return (true);
		}
		return (false);
	}
}

bool Response::checkLocation() 
{
	std::map<std::string, t_location> loc = this->_client->getServer()->getLocations();
	Request *req = this->_client->getRequest();
	std::string path = req->getPath();
	if (path.size() > 1 && path[path.size() - 1] != '/')
		path += "/";
	while (path.size() > 0)
	{
		std::map<std::string, t_location>::iterator it = loc.begin();
		std::map<std::string, t_location>::iterator ite = loc.end();
		while (it != ite)
		{
			if (path == it->first)
			{
				if (std::find(it->second.methods.begin(), it->second.methods.end(), req->getMethod()) == it->second.methods.end())
				{
					this->_statusCode = NOTALLOWED;
					std::string methods;
					std::vector<std::string>::iterator first = it->second.methods.begin();
					std::vector<std::string>::iterator last = it->second.methods.end();
					while (first != last)
					{
						methods += *first;
						first++;
						if (first != last)
							methods += ", ";
					}
					this->_headers["Allow"] = methods;
					writeResponse();
					return false;
				}
				this->_location = it->first;
				if (path.size() < req->getPath().size())
					path = req->getPath();
				this->_filename = path.substr(it->first.size(), std::string::npos);
				return true;
			}
			it++;
		}
		if (path.size() > 1 && path[path.size() - 1] != '/')
			path.erase(path.rfind('/') + 1, std::string::npos);
		else
			path.pop_back();
	}
	this->_statusCode = NOTFOUND;
	writeResponse();
	return false;
}

void Response::makeResponse()
{
	Request *req = this->_client->getRequest();
	std::map<std::string, t_location> loc = this->_client->getServer()->getLocations();
	if (!checkLocation())
		return ;
	if (req->getMethod() == "PUT")
	{
		int fd;
		DIR *dir;
		struct stat	stat;
		std::string root = loc[this->getLocation()].root;
		std::string name = this->getFileName();
		if (loc[this->getLocation()].max_body >= 0 && (int)req->getBody().size() > loc[this->getLocation()].max_body)
		{
			this->_statusCode = REQTOOLARGE;
			writeResponse();
			return;
		}
		if (root.size() > 1 && root[root.size() - 1] != '/')
			root.push_back('/');
		if (name.empty() || (dir = opendir((root + name).c_str())))
		{
			this->_statusCode = BADREQUEST;
			writeResponse();
			return ;
		}
		if (!lstat((root + name).c_str(), &stat))
			this->_statusCode = OK;
		else
			this->_statusCode = CREATED;
		try
		{
			if ((fd = open((root + name).c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0)
				throw ErrnoException();
		}
		catch(const std::exception& e)
		{
			this->_statusCode = INTERNALERROR;
			writeResponse();
			return ;
		}
		int cerr_save = dup(2);
		dup2(fd, 2);
		std::cerr << req->getBody();
		close(fd);
		dup2(cerr_save, 2);
		this->setDate();
		this->_headers["Last-Modified"] = this->getDate() + " GMT";
		this->_headers["Location"] = "/" + name;
		writeResponse();
		return;
	}
	else if (req->getMethod() == "HEAD")
	{
		this->_statusCode = OK;
		writeResponse();
	}
	else if (req->getMethod() == "GET")
	{
		if (loc[this->getLocation()].auth.first.empty())
		{
			if (!checkFile())
				this->_statusCode = NOTFOUND;
			else
				this->_statusCode = OK;
		}
		else
		{
			if (req->getHeaders().count("Authorization"))
			{
				if (req->getHeaders()["Authorization"].substr(6, std::string::npos) == base64encode(
					loc[this->getLocation()].auth.first + ":" + loc[this->getLocation()].auth.second))
				{
					if (!checkFile())
						this->_statusCode = NOTFOUND;
					else
					{
						this->_statusCode = OK;
						readBody(this->_fullpath);
					}
					writeResponse();
					return ;
				}
				else
					this->_statusCode = UNAUTHORIZED;
			}
			else
			{
				this->_statusCode = UNAUTHORIZED;
				this->_headers["WWW-Authenticate"] = "Basic rеаlm=\"test\"";
			}
		}
		writeResponse();
	}
	else if (req->getMethod() == "POST")
	{
		if (loc[this->getLocation()].max_body >= 0 && (int)req->getBody().size() > loc[this->getLocation()].max_body)
		{
			this->_statusCode = REQTOOLARGE;
			writeResponse();
			return;
		}
		if ((!loc[this->getLocation()].php.empty() && this->getFileName().rfind('.') != std::string::npos &&
		this->getFileName().substr(this->getFileName().rfind('.'), std::string::npos) == ".php") ||
		((!loc[this->getLocation()].exec.empty() && ((!loc[this->getLocation()].CGI.empty() && this->getFileName().rfind('.') != std::string::npos &&
		this->getFileName().substr(this->getFileName().rfind('.'), std::string::npos) == loc[this->getLocation()].CGI) || loc[this->getLocation()].CGI.empty()))))
		{
			try
			{
				req->doCGI(this->getLocation());
				req->parseCGI();
			}
			catch (std::exception &e)
			{
				this->_statusCode = INTERNALERROR;
				writeResponse();
				return ;
			}
			this->_body = req->getBody();
			if (this->_statusCode.empty())
				this->_statusCode = OK;
			writeResponse();
		}
		else
		{
			if (!checkFile())
				this->_statusCode = NOTFOUND;
			else
			{
				this->_statusCode = OK;
				this->_body = req->getBody();
			}
			writeResponse();
		}
	}
	else
	{
		this->_statusCode = NOTIMPLEMENTED;
		writeResponse();
	}
}

std::string &Response::getResponse()
{
	return this->_response;
}

void Response::setResponse(std::string const &msg)
{
	this->_response = msg;
}