/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convertConfig.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mskinner <v.golskiy@ya.ru>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 11:37:37 by mskinner          #+#    #+#             */
/*   Updated: 2021/03/29 13:03:24 by mskinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

int	ft_isalpha(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1);
	return (0);
}

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
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

int			ft_atoi(const char *str)
{
	int	sign;

	sign = 1;
	str = ft_rewind(str);
	if (*str == '-')
		sign = -1;
	if ((*str == '-') || (*str == '+'))
		str++;
	return (sign * ft_get_num(str, sign));
}

int	is_all_numbers(std::string s) {
	std::string::iterator	it;

	for (it = s.begin(); it != s.end(); ++it)
		if (!ft_isdigit(*it))
			return (0);
	return (1);
}

//TODO: verify htons is avaliable function / add source code
/*
** After verification that port consist of numbers
** we starting convertion: string -> char* -> integer -> unsigned short
** then pushing result to ports vector
** In case of wrong symbols error occurs
*/
void	Config::convert_port(void) {
	std::list<std::string>::iterator it;

	for (size_t i = 0; i < _servers.size(); ++i) {
		for (it = _servers[i].port.begin(); it != _servers[i].port.end(); it++) {
			if (is_all_numbers(*it))
				g_config.server[i]->port.push_back(htons(atoi((*it).c_str())));
			else
				error_message("Only numbers are allowed in ports naming");
		}
	}
}


/*
** Global configuration is stored on HEAP
*/
void	Config::init_global_configuration(void) {
	for (size_t i = 0; i < _servers.size(); ++i) {
		t_server_global *server = new t_server_global;

		server->name = _servers[i].name;
		server->location = _servers[i].location;
		server->host = _servers[i].host;
		server->error_page = _servers[i].error_page;
		g_config.server.push_back(server);
	}
	convert_port();
}