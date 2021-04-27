/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbarbera <fbarbera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/03 20:14:33 by fbarbera          #+#    #+#             */
/*   Updated: 2021/03/21 16:00:52 by fbarbera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_config.hpp"

int		ft_skip_spases(std::string::iterator i)
{
	int num = 0;
	while (*i == ' ' || *i == '\t' || *i == '\n')
	{
		num++;
		i++;
	}
	return num;
}

std::string		delete_comments(std::string line)
{
	return line.substr(0, line.find('#'));
}

int		found_block(std::string str, std::string::iterator i)
{
	size_t j;
	j = 0;
	while (j < str.size())
	{
		if (str[j] != *i)
			return 0;
		j++;
		i++;
	}
	return (1);
}

int		ft_spases_p(char c)
{
	if (c == ' ' || c == '\n' || c == '\t')
		return 1;
	return 0;
}

void	ft_error_p()
{
	std::cout << "ERROR" << std::endl;
	exit(1);
}


int				found_last_bracket(std::string::iterator i, std::string::iterator end)
{
	int num = 0;
	int count = 0;
	i+=6;
	num+=6;
	while (i != end && *i != '{')
	{
		if (ft_spases_p(*i))
		{
			i++;
			num++;
		}
		else 
			ft_error_p();
	}
	if (i == end)
		return (-1);
	count++;
	i++;
	num++;
	while (i != end && count != 0)
	{
		if (*i == '}')
			count--;
		if (*i == '{')
			count++;
		i++;
		num++;
	}
	if (count == 0)
		return num;
	else
		return (-1);
}

int				found_last_location(std::string::iterator i, std::string::iterator end)
{
	int num = 0;
	int count = 0;
	int word = 0;
	i+=8;
	num+=8;
	while (i != end && *i != '{')
	{
		if (ft_spases_p(*i))
		{
			i++;
			num++;
		}
		else if (word == 1) 
			ft_error_p();
		else
		{
			while (ft_spases_p(*i) == 0)
			{
				i++;
				num++;
			}
			word++;
		}
	}
	if (i == end)
		return (-1);
	count++;
	i++;
	num++;
	while (i != end && count != 0)
	{
		if (*i == '}')
			count--;
		if (*i == '{')
			count++;
		i++;
		num++;
	}
	if (count == 0)
		return num;
	else
		return (-1);
}

std::string		my_substr(std::string::iterator i, int len)
{
	std::string str;
	int n=0;

	while (n < len)
	{
		str += *i;
		i++;
		n++;
	}
	return str;
}

std::string		my_substr(std::string::iterator i, std::string::iterator end)
{
	std::string str;

	while (i != end)
	{
		str += *i;
		i++;
	}
	return str;
}

int found_last_newline(std::string::iterator i, std::string::iterator end)
{
	int num;
	num = 0;
	while (i != end && *i != '\n')
	{
		num++;
		i++;
	}
	return num;
}

std::string	ft_trim_spases(std::string s)
{
	std::string str;
	std::string::iterator i = s.begin();
	std::string::iterator end = s.end();
	while (ft_spases_p(*i))
		i++;
	if (i == end)
		return "";
	end--;
	while (ft_spases_p(*end))
		end--;
	end++;
	str = my_substr(i, end);
	return str;
}

std::string::iterator my_find(std::string &s, std::string des)
{
	std::string::iterator i = s.begin();
	size_t pos = s.find(des);
	
	if (pos != std::string::npos)
		i+=pos;
	else 
		i = s.end();
	return i;
}

std::vector<std::string> ft_split_string_to_vector(std::string str, char ch)
{
	std::vector<std::string> vector;
	std::string new_str;
	for (size_t index = 0; index < str.length(); index++)
	{
		if (str[index] == ch)
		{
			if (!new_str.empty())
				vector.push_back(new_str);
			new_str.clear();
		}
		else
			new_str += str[index];
	}
	if (!new_str.empty())
		vector.push_back(new_str);
	return (vector);
}

bool ft_isnumstring(std::string str)
{
	for (size_t i = 0; i < str.size(); i++)
	{
		if (!isnumber(str[i]))
			return false;
	}
	return true;
}

void	add_last_slash(std::string &str)
{
	if (*str.rbegin() != '/')
		str+='/';
}

void	add_first_slash(std::string &str)
{
	if (*str.begin() != '/')
		str = "/" + str;
}
