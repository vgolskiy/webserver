/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mskinner <v.golskiy@yandex.ru>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 14:10:05 by mskinner          #+#    #+#             */
/*   Updated: 2021/03/24 15:48:51 by mskinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#ifndef _CONFIG_HPP_
# define _CONFIG_HPP_

# include <stdlib.h>
# include <iostream>
# include <string>
# include <errno.h>
# include <fcntl.h>
# include <unistd.h>
# include <vector>

//GET NEXT LINE
# define	BUFFER_SIZE	1024

//PARSER CHARACTERS SET
# define	SPACES " \f\n\r\t\v"

//SERVER KEYS
# define	HOST 		"host"
# define	NAME 		"name"
# define	PORT 		"listen"
# define	ERR_PAGE 	"error"

//LOCATION KEYS
# define	METHOD 		"method"
# define	ROOT 		"root"
# define	INDEX 		"index"
# define	CGI_PATH 	"cgi_path"
# define	PHP_PATH 	"php_path"
# define	CGI 		"cgi"
# define	AUTO_INDEX 	"auto_index"
# define	MAX_BODY 	"max_body"
# define	AUTH 		"auth"

//testing
template <typename T>
void    printContainer(T &cont) {
	typename T::iterator it;

	std::cout << " [";
	for (it = cont.begin(); it != cont.end(); ++it)
	{
		if (it != cont.begin())
			std::cout << ", ";
		std::cout << *it;
	}
	std::cout << "]";
}

typedef struct					s_location
{
	std::string					index;
	std::string					root;
	std::vector<std::string>	method;
	std::string					uri;
	std::string					php_path;
	std::string					cgi;
	std::string					cgi_path;
	std::string					auth;
	int							auto_index;
	int							max_body;
	s_location() : auto_index(-1), max_body(-1) {}
}								t_location;

//Default config parsing is used instead of initiation function
typedef struct					s_server
{
	std::string					name;
	std::string					host;
	std::string					port;
	std::string					error_page;
	std::vector<t_location> 	location;
}								t_server;

class Config {
private:
	std::vector<std::string>	_raw_config;
	std::vector<t_server>		_servers;

public:
	Config(void) : _raw_config(), _servers() {};
	Config(const Config &copy) {
		*this = copy;
	};
	~Config(void) {};
	Config	operator=(const Config &other) {
		if (this != &other) {
			_raw_config = other._raw_config;
			_servers = other._servers;
		}
		return (*this);
	};

	void	parse_configuration_file(std::vector<std::string> file_lines);
	void	parse_server_data(std::vector<std::string> &to_parse, int index);

private:
	std::vector<std::string>	parse_line(std::string line);
};

void						*ft_memset(void *dest, int c, size_t len);
void						*ft_calloc(size_t nmemb, size_t size);
int							ft_isspace(int c);
std::string					read_file(const char *file_path);
std::vector<std::string>	split_to_lines(const std::string &s, const std::string &delimiter);

#endif
