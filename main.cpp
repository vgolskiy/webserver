/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mskinner <v.golskiy@ya.ru>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 14:24:50 by mskinner          #+#    #+#             */
/*   Updated: 2021/04/06 17:16:31 by mskinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include "Socket.hpp"
#include "Server.hpp"

void	init_servers(void); // utils.cpp
void	signals(void);
int 	select_loop(void);

int		main() {
	const char* 				file = CONFIG;
	std::string					file_content;
	Config						config;
	std::vector<std::string>	file_lines;

	try {
		file_content = read_file(file);
	}
	catch (int e) {
		if (e == -1)
			std::cerr << "Malloc failed" << std::endl;
		else if (e == -2)
			std::cerr << file << ": file is empty" << std::endl;
		else if (e == -3)
			std::cerr << file << ": can not read file" << std::endl;
		else
			std::cerr << file << ": " << strerror(e) << std::endl;
		return (EXIT_FAILURE);
	}
	file_lines = split(file_content, "\n");
	if (config.parse_configuration_file(file_lines))
		return (EXIT_FAILURE);

	init_servers();
	signals();	
	select_loop();
	
	return (EXIT_SUCCESS);
}
