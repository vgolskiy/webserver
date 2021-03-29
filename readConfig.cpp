/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readConfig.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mskinner <v.golskiy@ya.ru>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 11:16:22 by mskinner          #+#    #+#             */
/*   Updated: 2021/03/29 12:36:10 by mskinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

void	*ft_memset(void *dest, int c, size_t len)
{
	unsigned char	*dest_str;

	if (len == 0)
		return (dest);
	dest_str = (unsigned char *)dest;
	while (len--)
		*dest_str++ = (unsigned char)c;
	return (dest);
}

void	*ft_calloc(size_t nmemb, size_t size)
{
	unsigned char	*res;

	if (!(res = (unsigned char *)malloc(size * nmemb)))
		return (NULL);
	ft_memset(res, 0, nmemb * size);
	return ((void *)res);
}

int		ft_isspace(int c)
{
	if ((c == 32) || ((c >= 9) && (c <= 13)))
		return (1);
	return (0);
}

std::string	read_file(const char *file_path) {
	std::string		res;
	int				was_read;
	int				fd;
	char			*buf;

	fd = open(file_path, O_RDONLY);
	if (errno)
		throw (errno);
	if (!(buf = (char *)ft_calloc(BUFFER_SIZE + 1, sizeof(char))))
		throw (-1);
	while ((was_read = read(fd, buf, BUFFER_SIZE))> 0) {
		buf[was_read] = '\0';
		res.append(buf);
	}
	if (was_read < 0)
		throw (errno);
	free(buf);
	close(fd);
	return (res);
};

std::vector<std::string>	split_to_lines(const std::string &s, const std::string &delimiter) {
	std::vector<std::string>	res;
	size_t						pos;
	size_t						prev;

	prev = 0;
	while ((pos = s.find(delimiter, prev)) != std::string::npos) {
		res.push_back(s.substr(prev, pos - prev));
		prev = pos + 1;
	}
	// To get the last substring (or only, if delimiter is not found)
    res.push_back(s.substr(prev));
	return (res);
}

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

//TODO: file_content verification {} before parsing - Adelina

//Need to clear server and location after each push
void		Config::clear_server(t_server &server) {
	server.error_page.clear();
	server.host = "";
	server.name = "";
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

/*
** File content line by line is parsed into server
** or server location parameters
** In case of no location provided error message is shown
** ascii 35 #, 123 {, 125 }
*/
void		Config::parse_configuration_file(std::vector<std::string> &file_lines) {
	std::vector<std::string>::iterator	it;
	std::vector<std::string>			tokens;
	t_server							server;
	t_location							location;

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
					parse_servers_locations(tokens, location);
					it++;
					tokens = parse_line(*it);
				}
				server.location.push_back(location);
				clear_location(location);
			}
			else {
				error_message("Wrong server location parameters");
				while ((*it)[0] != 125)
					it++;
			continue ;
			}
		}
		else if (tokens[0][0] != 125)
			parse_servers_configurations(tokens, server);
		if (tokens[0][0] == 125)
		{
			_servers.push_back(server);
			clear_server(server);
		}
		// printContainer(tokens);
		// std::cout << std::endl;
	}
	config_check();
};

void	error_message(std::string message)
{
	std::cerr << message << std::endl;
	//TODO: exit if parse size or unknown/double tokens - invalid (close fd(?) + exit(1));
};

/*
** There could be a few servers configurations in one configuration file
** so we are using a vector container to store information
** If there were cases of double setting identification
** we are using the first one value only
** In case of no configuration file provided we are using the default one
** so _servers will always have at least one server inside
** ascii 35 #
*/
void	Config::parse_servers_configurations(std::vector<std::string> &to_parse, t_server &server) {
	if ((to_parse.size() == 1) || ((to_parse.size() > 2) && (to_parse[2][0] != 35)))
		error_message("Invalid arguments in configurations file");
	if ((to_parse[0] == HOST) && (!server.host.size())) //TODO: check how many listens should be
		server.host = to_parse[1];
	else if ((to_parse[0] == NAME) && (!server.name.size()))
		server.name = to_parse[1];
	else if ((to_parse[0] == PORT)) {
		for (size_t i = 1; i < to_parse.size(); ++i)
			server.port.push_back(to_parse[i]);
	}
	else if ((to_parse[0] == ERR_PAGE)) {
		for (size_t i = 1; i < to_parse.size(); ++i)
			server.error_page.push_back(to_parse[i]);
	}
	else
		error_message("Unknown or double parameter");
};

//ascii 123 {
void	Config::parse_servers_locations(std::vector<std::string> &to_parse, t_location &location) {
	if ((to_parse.size() == 1)
		|| ((to_parse.size() > 2) && (to_parse[2][0] != 35) && (to_parse[0] != METHOD)))
		error_message("Invalid arguments in configurations file");
	if ((to_parse[0] == METHOD)) {
		for (size_t i = 1; i < to_parse.size(); ++i)
			location.method.push_back(to_parse[i]);
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
};

// check number of listens port - not the same within one port
// check uri (so it starts with '/' only)
// check error pages order

void Config::config_check()
{
	std::list<std::string>::iterator it;

	for (size_t i = 0; i != _servers.size(); i++)
	{
		int check_uri = 0;
		for (int j = 0; j != _servers[i].location.size(); j++)
			if (_servers[i].location[j].uri.find_first_of("/", 0) != std::string::npos) //doesn't work though
				check_uri = 1;
		if (check_uri == 0)
			error_message("There should be at least one uri '/'");
		_servers[i].port.sort();
		for (it = _servers[i].port.begin(); it != --(_servers[i].port.end()); it++)
			if (*it == *(++it))
				error_message("The same ports are forbidden");
	}
};
