/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClassGenErrorPage.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbarbera <fbarbera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/11 19:38:39 by fbarbera          #+#    #+#             */
/*   Updated: 2021/03/13 19:32:57 by fbarbera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLASSGENERRORPAGE_HPP
# define CLASSGENERRORPAGE_HPP

# include "parser_config.hpp"
# include "map"
# include "system_include.hpp"

class ClassGenPage
{
private:
	std::string s1;
	std::string s2;
	std::map<int, std::string>	data;
	int found_error(int error, std::string &error_page);
	int count_dir(std::string str);
	ClassGenPage();
public:
	ClassGenPage(std::map<int, std::string> data);
	~ClassGenPage();
	std::string gen_error_page(int error);
	std::string gen_error_page(std::string error);
	std::string gen_auto_page(std::string path, std::string location);
};

# endif