/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convertConfig.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mskinner <v.golskiy@ya.ru>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 11:37:37 by mskinner          #+#    #+#             */
/*   Updated: 2021/04/01 20:05:46 by mskinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
t_config						g_config;

bool	ft_isalpha(int c) {
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (true);
	return (false);
}

bool	ft_isdigit(int c) {
	if (c >= '0' && c <= '9')
		return (true);
	return (false);
}

static char	*ft_rewind(const char *str)
{
	int i;

	i = 0;
	while (str[i] == '\t' || str[i] == '\v' || str[i] == '\f' ||
		str[i] == '\n' || str[i] == ' ' || str[i] == '\r')
		i++;
	return ((char *)str + i);
}

static int	ft_get_num(const char *str, int sign)
{
	int			i;
	long int	tmp;

	i = 0;
	tmp = 0;
	while (str[i] != '\0')
	{
		if (ft_isdigit(str[i]))
		{
			tmp = tmp * 10 + str[i] - '0';
			if (tmp < 0)
			{
				if (sign < 0)
					return (0);
				return (-1);
			}
			i++;
		}
		else
			break ;
	}
	return (tmp);
}

int			ft_atoi(const char *str) {
	int	sign;

	sign = 1;
	str = ft_rewind(str);
	if (*str == '-')
		sign = -1;
	if ((*str == '-') || (*str == '+'))
		str++;
	return (sign * ft_get_num(str, sign));
}

bool	is_all_numbers(std::string s) {
	std::string::iterator	it;

	for (it = s.begin(); it != s.end(); ++it)
		if (!ft_isdigit(*it))
			return (false);
	return (true);
}

/*
** After verification that port consist of numbers
** we starting convertion: string -> char* -> integer -> unsigned short
** then pushing result to ports vector
** In case of wrong symbols error occurs
*/
bool	Config::verify_port(std::string &s) {
	int	res;

	if (!is_all_numbers(s))
		return (false);
	res = atoi(s.c_str());
	if (res > USHRT_MAX || res < 1) // TODO: check if we should start with 1024
		return (false);
	return (true);
}

// Replacing "localhost" key name with IP "127.0.0.1"
std::string	Config::replace_localhost(std::string &s) {
	if (s == LOCALHOST)
		return (LOCALHOST_IP);
	return (s);
}

/*
** Host verification for:
** - number of IP parts (should be 4)
** - all parts consist of numbers only
** - all numbers in parts should be in range(0..255)
** - IPs "0.0.0.0" and "255.255.255.255" are forbidden (reserved)
*/
bool		Config::verify_localhost(std::string &s) {
	std::vector<std::string>			parts;
	std::vector<std::string>::iterator	it;
	int									n;

	parts = split(s, ".");
	if (parts.size() != 4)
		return (false);
	for (it = parts.begin(); it != parts.end(); ++it) {
		if (!is_all_numbers(*it))
			return (false);
		n = atoi((*it).c_str());
		if ((n < 0) || (n > 255))
			return (false);
	}
	if ((s == ZEROES_IP) || (s == BROADCAST_IP))
		return (false);
	return (true);
}

// Replacing,  verification  of host, host and port splitting
// In case of error empty line is returned
std::string	Config::convert_localhost(void) {
	unsigned short						port;
	std::string							res;
	std::vector<std::string>			tmp;
	std::list<unsigned short>::iterator	it;
	bool								add = true;

	for (size_t i = 0; i < _servers.size(); ++i) {
		res = "";
		if ((_servers[i].host.find(58)) == std::string::npos) {
			res = replace_localhost(_servers[i].host);
			if (!verify_localhost(res))
				return ("");
		else {
			tmp = split(_servers[i].host, ":");
			res = replace_localhost(tmp[0]);
			if (!verify_localhost(res))
				return ("");
			if (verify_port(tmp[1]))
				port = htons(atoi(tmp[1].c_str()));
			else
				return ("");
			for (it = _servers[i].port.begin(); it != _servers[i].port.end(); ++it) {
				if (*it == port) {
					add = false;
					break ;
				}
			}
			if (add)
				_servers[i].port.push_front(port);
		}
	}
}

/*
** Global configuration is stored on HEAP
** Making localhost verification for naming instead of IP (key name localhost vs 127.0.0.1)
** Additionally searching for port notation (localhost:port)
** Port is added to other ports if there was no such port mention before
** Adding new port to the top of the list (priority listening)
** ascii 58 :
*/
void	Config::init_global_configuration(void) {
	for (size_t i = 0; i < _servers.size(); ++i) {
		t_server *server = new t_server;

		server->name = _servers[i].name;
		server->location = _servers[i].location;
		server->host = _servers[i].host;
		server->error_page = _servers[i].error_page;
		g_config.server.push_back(server);
	}
}