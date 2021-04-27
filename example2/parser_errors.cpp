/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_errors.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbarbera <fbarbera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/08 17:23:42 by fbarbera          #+#    #+#             */
/*   Updated: 2021/03/18 17:52:27 by fbarbera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_errors.hpp"
# include <iostream>

void ft_exit(int error)
{
	std::cerr << "Config error:" << std::endl;
	switch (error)
	{
	case NO_METHOD:
		std::cerr << "no method" << std::endl;				break;
	case UN_METHOD:
		std::cerr << "unknown method" << std::endl;			break;
	case IP_ERROR:
		std::cerr << "ip error" << std::endl;				break;
	case NO_NAME:
		std::cerr << "No name =(" << std::endl;				break;
	case NO_PORT:
		std::cerr << "unknown port" << std::endl;			break;
	case DOUBLE_SL:
		std::cerr << "Double '/'" << std::endl;				break;
	case NO_LOCATION:
		std::cerr << "No location" << std::endl;			break;
	case MAX_SIZE_ERROR:
		std::cerr << "No valid max_body_size" << std::endl;	break;
	case NO_BRAKET:
		std::cerr << "Braket errors" << std::endl;			break;
	case ERROR_PAGE_ERROR:
		std::cerr << "Unknown argument in error_page"
										<< std::endl;		break;
	case PATH_EXTEN:
		std::cerr << "The number of paths is not equal to the number of extensions"
										<< std::endl;		break;
	case EXTENTION_DOT:
		std::cerr << "Еhe extension must start with a dot"
										<< std::endl;		break;
	case NO_VALID_AUTH_FILE:
		std::cerr << "auth_file must me\nAuthType Basic\nlogin:password"
										<< std::endl;		break;
	case UNICUE_IP:
		std::cerr << "IP+PORT not unicue" << std::endl;		break;
	case NO_ROOT:
		std::cerr << "No root" << std::endl; 				break;
	default:
		std::cerr << "Error unknown error\n";				break;
	}
	exit(1);
}

void ft_exit(int error, std::string s)
{
	std::cerr << "Config error:" << std::endl;
	switch (error)
	{
	case AUTH_PATH:
		std::cerr << "Error file ot path:\n";
		std::cerr << s << std::endl; break;
	default:
		std::cerr << "Error unknown error\n";			break;
	}
	exit(1);
}

void ft_exit(int error, std::string::iterator i)
{
	std::cerr << "Config error:" << std::endl;
	switch (error)
	{
	case ERROR_TOKEN:
		std::cerr << "Error line:\n";
		while (*i!='\n')
		{
			std::cerr << *i;
			i++;
		}
		std::cerr << std::endl; break;
	default:
		std::cerr << "Error unknown error\n";			break;
	}
	exit(1);
}