/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mskinner <v.golskiy@ya.ru>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 14:10:05 by mskinner          #+#    #+#             */
/*   Updated: 2021/03/30 00:00:05 by mskinner         ###   ########.fr       */
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
# include <stack>
# include <list>

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
// TODO: wildcards (?) https://www.prodevelopertutorial.com/wildcard-matching-in-c/
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
typedef struct					s_server
{
	std::string					name;
	std::string					host;
	std::list<std::string>		port;
	std::vector<std::string>	error_page;
	std::vector<t_location> 	location;
}								t_server;

//Server parameters structure for global configurations structure
typedef struct					s_server_global
{
	std::string					name;
	std::string					host;
	std::vector<unsigned short>	port;
	std::vector<std::string>	error_page;
	std::vector<t_location> 	location;
}								t_server_global;

//Global server configuration parameters structure
typedef struct						s_config
{
	std::vector<t_server_global*>	server;
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

	void	parse_configuration_file(std::vector<std::string> &file_lines);
	void	init_global_configuration(void);

private:
	std::vector<std::string>	parse_line(std::string &line);
	void						parse_servers_configurations(std::vector<std::string> &to_parse, t_server &server);
	void						parse_servers_locations(std::vector<std::string> &to_parse, t_location &location);
	void						clear_server(t_server &server);
	void						clear_location(t_location &location);
	void						config_check(void);
	std::string					verify_localhost(std::string &s);
	void						convert_port(void);
};

void						*ft_memset(void *dest, int c, size_t len);
void						*ft_calloc(size_t nmemb, size_t size);
int							ft_isspace(int c);
std::string					read_file(const char *file_path);
std::vector<std::string>	split_to_lines(const std::string &s, const std::string &delimiter);
void						error_message(std::string message);

#endif
