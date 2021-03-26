/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mskinner <v.golskiy@ya.ru>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 14:24:50 by mskinner          #+#    #+#             */
/*   Updated: 2021/03/25 13:11:13 by mskinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

int		check_brackets(std::vector<std::string> &lines)
{
	std::stack<std::string>		to_check;

	for (int i = 0; i != lines.size(); i++)
	{
		for (int j = 0; j != lines[i].size(); j++)
		{
			if (lines[i][j] == '{')
				to_check.push(&lines[i][j]);
			else if (lines[i][j] == '}' && !to_check.empty())
				to_check.pop();
			else if (lines[i][j] == '}' && to_check.empty())
			{
				std::cerr << "Invalid brackets!\n";
				return 1;
			}
		}
	}
	if (to_check.size())
	{
		std::cerr << "Invalid brackets!\n";
		return 1;
	}
	return 0;
};

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
	file_lines = split_to_lines(file_content, "\n");
	
	// CHECK BRACKETS
	std::vector<std::string>::iterator it;
	if (check_brackets(file_lines) == 1)
		return 0;
	
	config.parse_configuration_file(file_lines);
	return (EXIT_SUCCESS);
}
