/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mskinner <v.golskiy@ya.ru>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 14:24:50 by mskinner          #+#    #+#             */
/*   Updated: 2021/03/31 21:18:06 by mskinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

int select_loop(Config config)
{
	fd_set	read_fd_sets;
	fd_set	write_fd_sets;
	
	// TODO: разобраться с select
	
}

int main() {
	const char* 				file = CONFIG;
	std::string					file_content;
	Config						config;
	std::vector<std::string>	file_lines;

	try {
		file_content = read_file(file);
	}
	catch (int e) {
		if (e < 0)
			std::cerr << "Malloc failed" << std::endl;
		else
			std::cerr << file << ": " << strerror(e) << std::endl;
	}
	file_lines = split(file_content, "\n");
	if (config.parse_configuration_file(file_lines))
		return (EXIT_FAILURE);
	else
		return (EXIT_FAILURE);
	select_loop(config);
	return (EXIT_SUCCESS);
}
