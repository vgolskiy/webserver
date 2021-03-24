/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mskinner <v.golskiy@ya.ru>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 14:24:50 by mskinner          #+#    #+#             */
/*   Updated: 2021/03/24 01:00:57 by mskinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

int main() {
	const char* file = "nginx.conf";
	std::string	file_content;
	Config		config;

	try {
		file_content = read_file(file);
	}
	catch (int e) {
		if (e < 0)
			std::cerr << "Malloc failed" << std::endl;
		else
			std::cerr << file << ": " << strerror(e) << std::endl;
	}
	config.parse_configuration_file(split_to_lines(file_content, "\n"));
	return (EXIT_SUCCESS);
}
