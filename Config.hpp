/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mskinner <v.golskiy@ya.ru>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 14:10:05 by mskinner          #+#    #+#             */
/*   Updated: 2021/04/01 15:35:37 by mskinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#ifndef _CONFIG_HPP_
# define _CONFIG_HPP_

//Configuration
# include <stdlib.h>
# include <limits>
# include <iostream>
# include <string>
# include <errno.h>
# include <fcntl.h>
# include <unistd.h>
# include <vector>
# include <stack>
# include <list>

//Server
#include <sys/socket.h>
#include <netinet/in.h>

//GET NEXT LINE
# define	BUFFER_SIZE	1024

//PARSER CHARACTERS SET
# define	SPACES		" \f\n\r\t\v"

//DEFAULT CONFIG FILE
# define	CONFIG		"default"

//SERVER KEYS
# define	SERVER		"server"
// The parameter to server_name can be a full (exact) name, a wildcard, or a regular expression.
// REGEX is not the subject of this project
// If the Host header field does not match a server name, NGINX Plus routes
// the request to the default server for the port on which the request arrived.
# define	HOST 		"host"
# define	NAME 		"name"
# define	ERR_PAGE 	"error_page"
// To make an internal redirect when a file is not found.
// we can return a custom page along with an error code,
// substitute a different error code in the response, or redirect the browser to a different URI
# define	PORT 		"listen" 
// If a port is omitted, 
// the standard port is used. 
// Likewise, if an address is omitted, the server listens on all addresses. 
// If the listen directive is not included at all, 
// the “standard” port is 80/tcp and the “default” port is 8000/tcp
// Ports cannot be the same within one server
# define	LOCALHOST	"localhost"
# define	LOCALHOST_IP	"127.0.0.1"
# define	ZEROES_IP		"0.0.0.0"
# define	BROADCAST_IP	"255.255.255.255"

//LOCATION KEYS
# define	LOCATION	"location"
// Inside each location block, it is usually possible (with a few exceptions)
// to place even more location directives to further refine the processing for specific groups of requests.
// In the location:  prefix strings (pathnames /) and regular expressions (~) or case-insensitive (~*)
# define	METHOD 		"method"
# define	ROOT 		"root"
# define	INDEX 		"index"
// The index directive defines the index file’s name (the default value is index.html).	
# define	CGI_PATH 	"cgi_path"
# define	PHP_PATH 	"php_path"
# define	CGI 		"cgi"
# define	AUTO_INDEX 	"autoindex"
# define	MAX_BODY 	"max_body" // should limit client body size
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
typedef struct						s_server
{
	std::string						name;
	std::vector<std::string>		error_page;
	std::vector<t_location> 		location;
	std::vector<struct sockaddr_in> address; //address includes htons(port) and htonl(ip)
}									t_server;

//Server parameters structure for global configurations structure

//Global server configuration parameters structure
typedef struct						s_config
{
	std::vector<t_server*>			server;
}									t_config;

//Allowing configuration be accessible withing multiple files
//https://stackoverflow.com/questions/3627941/global-variable-within-multiple-files
extern t_config						g_config;

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

	int		parse_configuration_file(std::vector<std::string> &file_lines);
	void	init_global_configuration(void);

private:
	std::vector<std::string>	parse_line(std::string &line);
	int							parse_servers_configurations(std::vector<std::string> &to_parse, t_server &server);
	int							parse_servers_locations(std::vector<std::string> &to_parse, t_location &location);
	void						clear_server(t_server &server);
	void						clear_location(t_location &location);
	bool						verify_brackets(std::vector<std::string> &lines);
	bool						verify_config(void);
	std::string					replace_localhost(std::string &s);
	bool						verify_localhost(std::string &s);
	bool						verify_port(std::string &s);
	std::string					convert_localhost(void);
};

void						*ft_memset(void *dest, int c, size_t len);
void						*ft_calloc(size_t nmemb, size_t size);
bool						ft_isspace(int c);
bool						ft_isdigit(int c);
bool						ft_isalpha(int c);
bool						is_all_numbers(std::string s);
int							ft_atoi(const char *str);
std::string					read_file(const char *file_path);
std::vector<std::string>	split(const std::string &s, const std::string &delimiter);
void						error_message(std::string message);

#endif
