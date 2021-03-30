#include "conf_parser.hpp"

static bool isWs(std::string::const_iterator const &it)
{
	if (*it == ' ' || *it == '\t' || *it == '\r' || *it == '\n')
		return true;
	return false;
}

static bool skipWs(
    std::string::const_iterator &it,
    std::string::const_iterator const &ite)
{
	while (it != ite)
	{
		if (*it == ' ' || *it == '\t' || *it == '\r' || *it == '\n')
			it++;
		else
			return true;
	}
	return false;
}

static bool isKeyWord(
    std::string::const_iterator it,
    std::string::const_iterator const &ite,
    std::string const &keyWord)
{
	for (size_t i = 0; i < keyWord.size(); ++it, ++i)
	{
		if (it == ite || *it != keyWord[i])
			return false;
	}
	return true;
}

static std::string::const_iterator getWordEnd(
	std::string::const_iterator it,
	std::string::const_iterator const &ite,
	std::string const &endSymSet,
	std::list<Serv*> &servers)
{
	int endSymSetLen = endSymSet.size();
	for (; it != ite; ++it)
	{
		for (int i = 0; i < endSymSetLen; ++i)
		{
			if (*it == endSymSet[i])
				return it;
		}
	}
	deleteServersShowErrorAndExit(servers, CONFNOTVALID);
	return ite;
}

static bool parseListenPort(
    std::string::const_iterator &it,
    std::string::const_iterator const &ite,
    std::list<Serv*> &servers)
{
	if (!isKeyWord(it, ite, "listen"))
		return false;
	it += 6;
	if (!isWs(it))
		deleteServersShowErrorAndExit(servers, CONFNOTVALID);
	Serv *server = servers.back();
	if (server->getListenPort() != -1)
		deleteServersShowErrorAndExit(servers, CONFNOTVALID);
	if (!skipWs(it, ite))
		deleteServersShowErrorAndExit(servers, CONFNOTVALID);
	int port;
	try
	{
		port = std::stoi(std::string(it, ite));
	}
	catch (std::exception &e)
	{
		deleteServersShowErrorAndExit(servers, CONFNOTVALID);
	}
	if (port < 1024 || port > 65535)
		deleteServersShowErrorAndExit(servers, CONFNOTVALID);
	server->setListenPort(port);
	while (std::isdigit(*it))
		it++;
	if (!skipWs(it, ite))
		deleteServersShowErrorAndExit(servers, CONFNOTVALID);
	if (*it == ';')
		it++;
	else
		deleteServersShowErrorAndExit(servers, CONFNOTVALID);

	return true;
}

static bool parseServerName(
	std::string::const_iterator &it,
	std::string::const_iterator const &ite,
	std::list<Serv*> &servers)
{
	if (!isKeyWord(it, ite, "server_name"))
		return false;
	it += 11;
	Serv *server = servers.back();
	if (server->getServerName().empty() == false || !isWs(it) || !skipWs(it, ite))
		deleteServersShowErrorAndExit(servers, CONFNOTVALID);
	std::string::const_iterator wordEnd = getWordEnd(it, ite, ";", servers);
	if (it == wordEnd)
		deleteServersShowErrorAndExit(servers, CONFNOTVALID);
	std::string::const_iterator itp = wordEnd - 1;
	while (isWs(itp))
		itp--;
	server->setServerName(std::string(it, itp + 1));
	it = wordEnd + 1;
	return true;
}

static bool parseErrorPath(
    std::string::const_iterator &it,
    std::string::const_iterator const &ite,
    std::list<Serv*> &servers)
{
	if (!isKeyWord(it, ite, "error"))
		return false;
	it += 5;
	Serv *server = servers.back();
	if (server->getErrorPath().empty() == false || !isWs(it) || !skipWs(it, ite))
		deleteServersShowErrorAndExit(servers, CONFNOTVALID);
	std::string::const_iterator wordEnd = getWordEnd(it, ite, ";", servers);
	if (it == wordEnd)
		deleteServersShowErrorAndExit(servers, CONFNOTVALID);
	std::string::const_iterator itp = wordEnd - 1;
	while (isWs(itp))
		itp--;
	server->setErrorPath(std::string(it, itp + 1));
	it = wordEnd + 1;
	return true;
}

static bool parseMethods(
	std::string::const_iterator &it,
	std::string::const_iterator const &ite,
	std::pair<std::string, t_location> &location,
	std::list<Serv*> &servers)
{
	if (!isKeyWord(it, ite, "methods"))
		return false;
	it += 7;
	if (location.second.methods.empty() == false)
		deleteServersShowErrorAndExit(servers, CONFNOTVALID);
	if (!isWs(it))
		deleteServersShowErrorAndExit(servers, CONFNOTVALID);
	if (!skipWs(it, ite))
		deleteServersShowErrorAndExit(servers, CONFNOTVALID);
	std::string::const_iterator wordEnd = getWordEnd(it, ite, ";", servers);
	if (it == wordEnd)
		deleteServersShowErrorAndExit(servers, CONFNOTVALID);
	std::string method;
	std::string::const_iterator itp = it;
	while (it != wordEnd)
	{
		while (!isWs(itp) && *itp != ',' && *itp != ';')
			itp++;
		method = std::string(it, itp);
		if (Http::isValidMethod(method))
			location.second.methods.push_back(method);
		else
			deleteServersShowErrorAndExit(servers, CONFNOTVALID);
		while (itp != ite && *itp != ';' && (isWs(itp) == true || *itp == ','))
			itp++;
		it = itp;
	}
	it = wordEnd + 1;
	return true;
}

static bool parseRoot(
	std::string::const_iterator &it,
	std::string::const_iterator const &ite,
	std::pair<std::string, t_location> &location,
	std::list<Serv*> &servers)
{
	if (!isKeyWord(it, ite, "root"))
		return false;
	it += 4;
	if (location.second.root.empty() == false || !isWs(it) || !skipWs(it, ite))
		deleteServersShowErrorAndExit(servers, CONFNOTVALID);
	std::string::const_iterator wordEnd = getWordEnd(it, ite, ";", servers);
	if (it == wordEnd)
		deleteServersShowErrorAndExit(servers, CONFNOTVALID);
	std::string::const_iterator itp = wordEnd - 1;
	while (isWs(itp))
		itp--;
	location.second.root = std::string(it, itp + 1);
	it = wordEnd + 1;
	return true;
}

static bool parseIndex(
	std::string::const_iterator &it,
	std::string::const_iterator const &ite,
	std::pair<std::string, t_location> &location,
	std::list<Serv*> &servers)
{
	if (!isKeyWord(it, ite, "index"))
		return false;
	it += 5;
	if (location.second.index.empty() == false || !isWs(it) || !skipWs(it, ite))
		deleteServersShowErrorAndExit(servers, CONFNOTVALID);
	std::string::const_iterator wordEnd = getWordEnd(it, ite, ";", servers);
	if (it == wordEnd)
		deleteServersShowErrorAndExit(servers, CONFNOTVALID);
	std::string::const_iterator itp = wordEnd - 1;
	while (isWs(itp))
		itp--;
	location.second.index = std::string(it, itp + 1);
	it = wordEnd + 1;
	return true;
}

static bool parseAutoindex(
	std::string::const_iterator &it,
	std::string::const_iterator const &ite,
	std::pair<std::string, t_location> &location,
	std::list<Serv*> &servers)
{
	if (!isKeyWord(it, ite, "autoindex"))
		return false;
	it += 9;
	if (location.second.autoindex == true || !isWs(it) || !skipWs(it, ite))
		deleteServersShowErrorAndExit(servers, CONFNOTVALID);
	std::string::const_iterator wordEnd = getWordEnd(it, ite, ";", servers);
	if (it == wordEnd)
		deleteServersShowErrorAndExit(servers, CONFNOTVALID);
	std::string::const_iterator itp = wordEnd - 1;
	while (isWs(itp))
		itp--;
	std::string value = std::string(it, itp + 1);
	if (value == "on")
		location.second.autoindex = true;
	else if (value == "off")
		location.second.autoindex = false;
	else
		deleteServersShowErrorAndExit(servers, CONFNOTVALID);
	it = wordEnd + 1;
	return true;
}

static bool parseCGI(
	std::string::const_iterator &it,
	std::string::const_iterator const &ite,
	std::pair<std::string, t_location> &location,
	std::list<Serv*> &servers)
{
	if (!isKeyWord(it, ite, "CGI"))
		return false;
	it += 3;
	if (location.second.CGI.empty() == false || !isWs(it) || !skipWs(it, ite))
		deleteServersShowErrorAndExit(servers, CONFNOTVALID);
	std::string::const_iterator wordEnd = getWordEnd(it, ite, ";", servers);
	if (it == wordEnd)
		deleteServersShowErrorAndExit(servers, CONFNOTVALID);
	std::string::const_iterator itp = wordEnd - 1;
	while (isWs(itp))
		itp--;
	location.second.CGI = std::string(it, itp + 1);
	if (location.second.CGI[0] != '.' ||
		std::count(location.second.CGI.begin(), location.second.CGI.end(), '.') > 1)
		deleteServersShowErrorAndExit(servers, CONFNOTVALID);
	it = wordEnd + 1;
	return true;
}

static bool parseExec(
	std::string::const_iterator &it,
	std::string::const_iterator const &ite,
	std::pair<std::string, t_location> &location,
	std::list<Serv*> &servers)
{
	if (!isKeyWord(it, ite, "exec"))
		return false;
	it += 4;
	if (location.second.exec.empty() == false || !isWs(it) || !skipWs(it, ite))
		deleteServersShowErrorAndExit(servers, CONFNOTVALID);
	std::string::const_iterator wordEnd = getWordEnd(it, ite, ";", servers);
	if (it == wordEnd)
		deleteServersShowErrorAndExit(servers, CONFNOTVALID);
	std::string::const_iterator itp = wordEnd - 1;
	while (isWs(itp))
		itp--;
	location.second.exec = std::string(it, itp + 1);
	it = wordEnd + 1;
	return true;
}

static bool parsePhp(
	std::string::const_iterator &it,
	std::string::const_iterator const &ite,
	std::pair<std::string, t_location> &location,
	std::list<Serv*> &servers)
{
	if (!isKeyWord(it, ite, "php"))
		return false;
	it += 3;
	if (location.second.php.empty() == false || !isWs(it) || !skipWs(it, ite))
		deleteServersShowErrorAndExit(servers, CONFNOTVALID);
	std::string::const_iterator wordEnd = getWordEnd(it, ite, ";", servers);
	if (it == wordEnd)
		deleteServersShowErrorAndExit(servers, CONFNOTVALID);
	std::string::const_iterator itp = wordEnd - 1;
	while (isWs(itp))
		itp--;
	location.second.php = std::string(it, itp + 1);
	it = wordEnd + 1;
	return true;
}

static bool parseAuth(
	std::string::const_iterator &it,
	std::string::const_iterator const &ite,
	std::pair<std::string, t_location> &location,
	std::list<Serv*> servers)
{
	if (!isKeyWord(it, ite, "auth"))
		return false;
	it += 4;
	if (location.second.auth.first.empty() == false ||
		location.second.auth.second.empty() == false ||
		!isWs(it) || !skipWs(it, ite))
		deleteServersShowErrorAndExit(servers, CONFNOTVALID);
	std::string::const_iterator wordEnd = getWordEnd(it, ite, ";", servers);
	if (it == wordEnd)
		deleteServersShowErrorAndExit(servers, CONFNOTVALID);
	std::string::const_iterator itp = wordEnd - 1;
	while (isWs(itp))
		itp--;
	if (std::count(it, itp, ':') != 1)
		deleteServersShowErrorAndExit(servers, CONFNOTVALID);
	std::string::const_iterator itwe = it;
	while (itwe != itp + 1 && *itwe != ':' && !isWs(itwe))
		itwe++;
	if (it == itwe)
		deleteServersShowErrorAndExit(servers, CONFNOTVALID);
	location.second.auth.first = std::string(it, itwe);
	it = getWordEnd(it, ite, ":", servers);
	it++;
	if (!skipWs(it, ite))
		deleteServersShowErrorAndExit(servers, CONFNOTVALID);
	itwe = it;
	while (itwe != itp + 1 && *itwe != ';' && !isWs(itwe))
		itwe++;
	if (it == itwe)
		deleteServersShowErrorAndExit(servers, CONFNOTVALID);
	location.second.auth.second = std::string(it, itwe);
	while (isWs(itwe))
		itwe++;
	if (*itwe != ';')
		deleteServersShowErrorAndExit(servers, CONFNOTVALID);
	it = wordEnd + 1;
	return true;
}

static bool parseMaxBody(
	std::string::const_iterator &it,
	std::string::const_iterator const &ite,
	std::pair<std::string, t_location> &location,
	std::list<Serv*> &servers)
{
	if (!isKeyWord(it, ite, "max_body"))
		return false;
	it += 8;
	if (!isWs(it))
		deleteServersShowErrorAndExit(servers, CONFNOTVALID);
	if (!skipWs(it, ite))
		deleteServersShowErrorAndExit(servers, CONFNOTVALID);
	if (location.second.max_body > -1)
		deleteServersShowErrorAndExit(servers, CONFNOTVALID);
	std::string::const_iterator wordEnd = getWordEnd(it, ite, ";", servers);
	if (it == wordEnd)
		deleteServersShowErrorAndExit(servers, CONFNOTVALID);
	std::string::const_iterator itp = wordEnd - 1;
	while (isWs(itp))
		itp--;
	std::string max_body = std::string(it, itp + 1);
	char const nums[] = "0123456789";
	for (size_t i = 0; i < max_body.size(); i++)
	{
		for (size_t j = 0; j < 10; j++)
		{
			if (max_body[i] == nums[j])
				break;
			if (j == 9)
				deleteServersShowErrorAndExit(servers, CONFNOTVALID);
		}
	}
	try
	{
		location.second.max_body = std::stoi(max_body);
	}
	catch (std::exception &e)
	{
		deleteServersShowErrorAndExit(servers, CONFNOTVALID);
	}
	it = wordEnd + 1;
	return true;
}

static bool parseLocation(
    std::string::const_iterator &it,
    std::string::const_iterator const &ite,
    std::list<Serv*> &servers)
{
	if (!isKeyWord(it, ite, "location"))
		return false;
	it += 8;
	if (!isWs(it) || !skipWs(it, ite))
		deleteServersShowErrorAndExit(servers, CONFNOTVALID);

	Serv *server = servers.back();
	std::pair<std::string, t_location> location;
	location.second.max_body = -1;
	location.second.autoindex = false;
	std::string::const_iterator wordEnd = getWordEnd(it, ite, " \t\r\n{", servers);
	if (it == wordEnd)
		deleteServersShowErrorAndExit(servers, CONFNOTVALID);
	location.first = std::string(it, wordEnd);
	if (server->getLocations().count(location.first))
		deleteServersShowErrorAndExit(servers, CONFNOTVALID);
	if (*wordEnd != '{')
		wordEnd = getWordEnd(wordEnd, ite, "{", servers);
	it = wordEnd + 1;
	while (skipWs(it, ite) == true)
	{
		if (*it == '}')
			break;
		if (parseMethods(it, ite, location, servers))
			continue;
		if (parseRoot(it, ite, location, servers))
			continue;
		if (parseIndex(it, ite, location, servers))
			continue;
		if (parseAutoindex(it, ite, location, servers))
			continue;
		if (parseCGI(it, ite, location, servers))
			continue;
		if (parseExec(it, ite, location, servers))
			continue;
		if (parsePhp(it, ite, location, servers))
			continue;
		if (parseAuth(it, ite, location, servers))
			continue;
		if (parseMaxBody(it, ite, location, servers))
			continue;
		deleteServersShowErrorAndExit(servers, CONFNOTVALID);
	}
	if (*it != '}')
		deleteServersShowErrorAndExit(servers, CONFNOTVALID);
	it++;
	server->addLocation(location);
	return true;
}

std::list<Serv*> parseConfFile(std::string const &config)
{	
	std::list<Serv*> servers;
	std::string::const_iterator it = config.begin();
	std::string::const_iterator ite = config.end();
	for (; skipWs(it, ite) && it != ite; ++it)
	{
		if (isKeyWord(it, ite, "server"))
		{
			it += 6;
			if (!skipWs(it, ite) || *it != '{')
				deleteServersShowErrorAndExit(servers, CONFNOTVALID);
			it++;
			servers.push_back(new Serv());

			while (skipWs(it, ite) == true)
			{
				if (*it == '}')
					break;
				if (parseListenPort(it, ite, servers))
					continue;
				if (parseServerName(it, ite, servers))
					continue;
				if (parseErrorPath(it, ite, servers))
					continue;
				if (parseLocation(it, ite, servers))
					continue;
				deleteServersShowErrorAndExit(servers, CONFNOTVALID);
			}
			if (*it != '}')
				deleteServersShowErrorAndExit(servers, CONFNOTVALID);
		}
		else
			deleteServersShowErrorAndExit(servers, CONFNOTVALID);
	}
    
	return servers;
}
