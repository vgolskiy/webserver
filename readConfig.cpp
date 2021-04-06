/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readConfig.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mskinner <v.golskiy@ya.ru>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 11:16:22 by mskinner          #+#    #+#             */
/*   Updated: 2021/04/06 20:09:33 by mskinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

void	*ft_memset(void *dest, int c, size_t len) {
	unsigned char	*dest_str;

	if (len == 0)
		return (dest);
	dest_str = (unsigned char *)dest;
	while (len--)
		*dest_str++ = (unsigned char)c;
	return (dest);
}

void	*ft_calloc(size_t nmemb, size_t size) {
	unsigned char	*res;

	if (!(res = (unsigned char *)malloc(size * nmemb)))
		return (NULL);
	ft_memset(res, 0, nmemb * size);
	return ((void *)res);
}

bool	ft_isspace(int c) {
	if ((c == 32) || ((c >= 9) && (c <= 13)))
		return (true);
	return (false);
}

std::string		read_file(const char *file_path) {
	int				was_read;
	int				fd;
	char			*buf;
	struct stat 	file_statistics;
	std::string		res;

	fd = open(file_path, O_RDONLY);
	if (errno)
		throw (errno);
	if (fstat(fd, &file_statistics) == -1) {
		close(fd);
		throw (errno);
	}
	if (!file_statistics.st_size) {
		close(fd);
		throw (-2);
	}
	if (!(buf = (char *)ft_calloc(BUFFER_SIZE + 1, sizeof(char)))) {
		close(fd);
		throw (-1);
	}
	while ((was_read = read(fd, buf, BUFFER_SIZE))> 0) {
		buf[was_read] = '\0';
		res.append(buf);
	}
	if (was_read < 0) {
		close(fd);
		throw (-3);
	}
	free(buf);
	close(fd);
	return (res);
};

std::vector<std::string>	split(const std::string &s, const std::string &delimiter) {
	std::vector<std::string>	res;
	size_t						pos;
	size_t						prev;

	prev = 0;
	while ((pos = s.find(delimiter, prev)) != std::string::npos) {
		res.push_back(s.substr(prev, pos - prev));
		prev = pos + delimiter.length();
	}
	// To get the last substring (or only, if delimiter is not found)
    res.push_back(s.substr(prev));
	return (res);
}

Config::Config(const Config &copy) {*this = copy;};

Config	Config::operator=(const Config &other) {
		if (this != &other)
			_servers = other._servers;
		return (*this);
};

std::vector<std::string>	Config::parse_line(std::string &line) {
	std::vector<std::string>	res;
	//positions of parameter description symbols in line
	size_t						begin;
	size_t						end;

	end = 0;
	while (end < line.length()) {
		begin = line.find_first_not_of(SPACES, end);
		if (begin == std::string::npos)
			return (res);
		end = line.find_first_of(SPACES, begin);
		if (end == std::string::npos)
			end = line.length();
		res.push_back(line.substr(begin, (end - begin)));
	}
	return (res);
};

//Need to clear server and location after each push
void		Config::clear_server(t_server &server) {
	server.error_page.clear();
	server.host = "";
	server.name.clear();
	server.port.clear();
	server.location.clear();
};

void		Config::clear_location(t_location &location) {
	location.auth = "";
	location.auto_index = -1;
	location.cgi = "";
	location.cgi_path = "";
	location.index = "";
	location.max_body = -1;
	location.method.clear();
	location.php_path = "";
	location.root = "";
	location.uri = "";
};

//Lists for parsing
std::string const	Config::_methods[] = {
	"GET",
	"HEAD",
	"POST",
	"PUT",
	"DELETE",
	"CONNECT",
	"OPTIONS",
	"TRACE",
	"PATCH"
};

std::string const	Config::_protocols[] = {
	"HTTP/1.1"
};

std::string const	Config::_headers[] = {
	"Accept-Charsets",
	"Accept-Language",
	"Allow",
	"Authorization",
	"Content-Language",
	"Content-Length",
	"Content-Location",
	"Content-Type",
	"Date",
	"Host",
	"Last-Modified",
	"Location",
	"Referer",
	"Retry-After",
	"Server",
	"Transfer-Encoding",
	"User-Agent",
	"WWW-Authenticate"
};


/*
** File content line by line is parsed into server
** or server location parameters
** In case of no location provided error message is shown
** ascii 35 #, 123 {, 125 }
*/
int		Config::parse_configuration_file(std::vector<std::string> &file_lines) {
	std::vector<std::string>::iterator	it;
	std::vector<std::string>			tokens;
	t_server							server;
	t_location							location;

	if (!verify_brackets(file_lines))
		return (EXIT_FAILURE);
	for (it = file_lines.begin(); it != file_lines.end(); ++it) {
		tokens = parse_line(*it);
		if ((!tokens.size()) || (tokens[0][0] == 35)
			|| ((tokens[0] == SERVER) && (tokens[1][0] == 123)))
			continue ;
		if (tokens[0] == LOCATION) {
			if (((tokens.size() == 3) && (tokens[2][0] == 123))
				|| ((tokens.size() == 2) && (tokens[1][tokens[1].length() - 1] == 123))) {
				location.uri = tokens[1];
				it++;
				tokens = parse_line(*it);				
				while (tokens[0][0] != 125) {
					if (parse_servers_locations(tokens, location))
						return (EXIT_FAILURE);
					it++;
					tokens = parse_line(*it);
				}
				server.location.push_back(location);
				clear_location(location);
			}
			else {
				error_message("Wrong server location parameters");
				return (EXIT_FAILURE);
			}
			continue ;
		}
		else if (tokens[0][0] != 125) {
			if (parse_servers_configurations(tokens, server))
				return (EXIT_FAILURE);
		}
		if (tokens[0][0] == 125) {
			if (convert_localhost(server.host, server))
				return (EXIT_FAILURE);
			_servers.push_back(server);
			clear_server(server);
		}
	}
	if (!verify_config())
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
};

void	error_message(std::string message)
{
	std::cerr << message << std::endl;
};

/*
** There could be a few servers configurations in one configuration file
** so we are using a vector container to store information
** If there were cases of double setting identification
** we are using the first one value only
** In case of no configuration file provided we are using the default one
** so _servers will always have at least one server inside
** ascii 35 # 59 ; 58 :
*/
int		Config::parse_servers_configurations(std::vector<std::string> &to_parse, t_server &server) {
	if ((to_parse.size() == 1) || ((to_parse.size() > 2) && (to_parse[2][0] != 35) 
		&& (to_parse[0] != NAME))) {
		error_message("Invalid arguments in configurations file");
		return (EXIT_FAILURE);
	}
	if (to_parse[to_parse.size() - 1].back() != 59) {
		error_message("directive " + to_parse[0] + " is not terminated by ;");
		return (EXIT_FAILURE);
	}
	to_parse[to_parse.size() - 1] = to_parse[to_parse.size() - 1].erase(to_parse[to_parse.size() - 1].size()-1);
	if ((to_parse[0] == NAME) && (!server.name.size())) {
		for (size_t	i = 1; i != to_parse.size(); ++i)
			server.name.push_back(to_parse[i]);
	}
	else if (to_parse[0] == PORT) {
		if (verify_port(to_parse[1]))
			server.port.push_back(htons(atoi(to_parse[1].c_str())));
		else if (to_parse[1].find(58) != std::string::npos)
			server.host = to_parse[1];
		else {
			error_message("Invalid arguments in configurations file");
			return (EXIT_FAILURE);
		}
	}
	else if ((to_parse[0] == ERR_PAGE) && (!server.error_page.size())) {
		for (size_t i = 1; i < to_parse.size(); ++i)
			server.error_page.push_back(to_parse[i]);
	}
	else
		error_message("Unknown or double parameter");
	return (EXIT_SUCCESS);
};

bool	Config::verify_method(std::string &s) {
	for (int i = 0; i < N_ELEMS(_methods); ++i)
		if (_methods[i] == s)
			return (true);
	return (false);
}

//Searching for provided method among avaliable
int		Config::parse_method(t_location &location, std::string &s) {
	std::vector<std::string>	tmp;

	tmp = split(s, ",");
	for (size_t i = 0; i < tmp.size(); ++i) {
		if ((tmp[i].length()) && (verify_method(tmp[i])))
			location.method.push_back(tmp[i]);
		else {
			error_message("Directive methods is invalid");
			return (EXIT_FAILURE);
		}
	}
	return (EXIT_SUCCESS);
}

//ascii 123 {
int		Config::parse_servers_locations(std::vector<std::string> &to_parse, t_location &location) {
	if ((to_parse.size() == 1)
		|| ((to_parse.size() > 2) && (to_parse[2][0] != 35) && (to_parse[0] != METHOD))) {
		error_message("Invalid arguments in configurations file");
		return (EXIT_FAILURE);
	}
	if (to_parse[to_parse.size() - 1].back() != 59) {
		error_message("Directive " + to_parse[0] + " is not terminated by ;");
		return (EXIT_FAILURE);
	}
	to_parse[to_parse.size() - 1] = to_parse[to_parse.size() - 1].erase(to_parse[to_parse.size() - 1].size()-1);
	if ((to_parse[0] == METHOD) && (!location.root.size())) {
		to_parse[0] = "";
		for (size_t i = 1; i < to_parse.size(); ++i)
			to_parse[0]+=to_parse[i];
		if (parse_method(location, to_parse[0]))
			return (EXIT_FAILURE);
	}
	else if ((to_parse[0] == ROOT) && (!location.root.size()))
		location.root = to_parse[1];
	else if ((to_parse[0] == INDEX) && (!location.index.size()))
		location.index = to_parse[1];
	else if ((to_parse[0] == CGI_PATH) && (!location.cgi_path.size()))
		location.cgi_path = to_parse[1];
	else if ((to_parse[0] == PHP_PATH) && (!location.php_path.size()))
		location.php_path = to_parse[1];
	else if ((to_parse[0] == CGI) && (!location.cgi.size()))
		location.cgi = to_parse[1];
	else if ((to_parse[0] == AUTO_INDEX) && (location.auto_index == -1))
		location.auto_index = stoi(to_parse[1]);
	else if ((to_parse[0] == MAX_BODY) && (location.max_body == -1))
		location.max_body = stoi(to_parse[1]);
	else if ((to_parse[0] == AUTH) && (!location.auth.size()))
		location.auth = to_parse[1];
	else
		error_message("Unknown or double parameter");
	return (EXIT_SUCCESS);
};

//Verify that all brackets were closed
bool		Config::verify_brackets(std::vector<std::string> &lines)
{
	std::stack<std::string>		to_check;

	for (size_t i = 0; i != lines.size(); i++)
	{
		for (size_t j = 0; j != lines[i].size(); j++)
		{
			if (lines[i][j] == '{')
				to_check.push(&lines[i][j]);
			else if (lines[i][j] == '}' && !to_check.empty())
				to_check.pop();
			else if (lines[i][j] == '}' && to_check.empty())
			{
				std::cerr << "Invalid brackets\n";
				return (false);
			}
		}
	}
	if (to_check.size())
	{
		std::cerr << "Invalid brackets\n";
		return (false);
	}
	return (true);
};

// check number of listens port - not the same within one port
// check uri (so it starts with '/' only)
// check error pages order

bool	Config::verify_config()
{
	std::list<unsigned short>::iterator it;

	for (size_t i = 0; i != _servers.size(); i++)
	{
		if (!_servers[i].host.length())
			_servers[i].host = LOCALHOST_IP;
		if (!_servers[i].port.size())
			return (false);
		if (!_servers[i].name.size())
			_servers[i].name.push_back(SERVER_NAME);
		if (!_servers[i].port.size())
			return (false);
		_servers[i].port.sort();
		for (it = _servers[i].port.begin(); it != --(_servers[i].port.end()); it++) {
			if (*it == *(++it)) {
				error_message("The same ports are forbidden");
				return (false);
			}
		}
	}
	return (true);
};
