/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mskinner <v.golskiy@ya.ru>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 14:10:05 by mskinner          #+#    #+#             */
/*   Updated: 2021/03/24 02:39:08 by mskinner         ###   ########.fr       */
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

# define	BUFFER_SIZE	1024
# define	SPACES " \f\n\r\t\v"

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

class Config {
private:
	std::vector<std::string>	_raw_config;

public:
	Config(void) : _raw_config() {};
	Config(const Config &copy) {
		*this = copy;
	};
	~Config(void) {};
	Config	operator=(const Config &other) {
		if (this != &other) {
			_raw_config = other._raw_config;
		}
		return (*this);
	};

	void	parse_configuration_file(std::vector<std::string> file_lines);

private:
	std::vector<std::string>	parse_line(std::string line);
};

void						*ft_memset(void *dest, int c, size_t len);
void						*ft_calloc(size_t nmemb, size_t size);
int							ft_isspace(int c);
std::string					read_file(const char *file_path);
std::vector<std::string>	split_to_lines(const std::string &s, const std::string &delimiter);

#endif
