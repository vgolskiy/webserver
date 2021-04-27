/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClassParser.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbarbera <fbarbera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/02 17:58:29 by fbarbera          #+#    #+#             */
/*   Updated: 2021/03/21 16:08:51 by fbarbera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClassParser.hpp"

void	ClassParser::fing_config_file()
{
	if (0) //error
		exit(1);
	this->config_addr = "webserv.conf";
}

void ClassParser::read_from_file()
{	
	std::string line;
 
    std::ifstream in(config_addr); // окрываем файл для чтения
    if (in.is_open())
    {
        while (getline(in, line))
			this->line_data = line_data + "\n" + delete_comments(line);
    }
	else
	{
		//std::cout << "error open" << std::endl; //error
		exit(1);
	}
    in.close();     // закрываем файл
}

std::vector<std::string> ClassParser::split_servers()
{
	std::string::iterator i;
	std::string::iterator end;
	std::vector<std::string> lines_for_server;
	int		len_of_one_server;
	i = line_data.begin();
	end = line_data.end();
	i += ft_skip_spases(i);
	while (i != line_data.end())
	{
		if (found_block("server ", i) == 1)
		{
			len_of_one_server = found_last_bracket(i, end);
			if (len_of_one_server == -1)
				ft_exit(NO_BRAKET);
			lines_for_server.push_back(my_substr(i, len_of_one_server));
			i+=len_of_one_server;
		}
		else if (*i == ' ' || *i == '\n' || *i == '\t')
			i++;
		else
			ft_error_p();
	}
	return lines_for_server;
}

std::vector<std::string> set_error_page(std::string &str, std::string found)
{
	std::vector<std::string> names;
	std::string s_line;
	std::string::iterator i;
	std::string::iterator end;
	int		len_of_one_server;
	size_t index;
	i = str.begin();
	end = str.end();
	i += ft_skip_spases(i);
	while (i != str.end())
	{
		if (found_block(found, i) == 1)
		{
			len_of_one_server = found_last_newline(i, end);
			s_line = my_substr(i, len_of_one_server);
			index = i - str.begin();
			str = my_substr(str.begin(), i) + my_substr(i + len_of_one_server, end);
			end = str.end();
			i = str.begin() + index;
			names.push_back(s_line);
		}
		else
			i++;
	}
	return names;
}

std::vector<std::string> set_vector(std::string &str, std::string found)
{
	std::vector<std::string> names;
	std::string s_line;
	std::string::iterator i;
	std::string::iterator end;
	int		len_of_one_server;
	i = str.begin();
	end = str.end();
	i += ft_skip_spases(i);
	while (i != str.end())
	{
		if (found_block(found, i) == 1)
		{
			len_of_one_server = found_last_newline(i, end);
			s_line = my_substr(i, len_of_one_server);
			str = my_substr(str.begin(), i) + my_substr(i + len_of_one_server, end);
			names.push_back(s_line);
			break;
		}
		else
			i++;
	}
	return names;
}

std::string set_string_p(std::string &str, std::string found)
{
	std::string s_line;
	std::string::iterator i;
	std::string::iterator end;
	int		len_of_one_server;
	i = str.begin();
	end = str.end();
	i += ft_skip_spases(i);
	while (i != str.end())
	{
		if (ft_spases_p(*i) == 1)
			i++;
		else if (found_block(found, i) == 1)
		{
			len_of_one_server = found_last_newline(i, end);
			s_line = my_substr(i, len_of_one_server);
			str = my_substr(str.begin(), i) + my_substr(i + len_of_one_server, end);
			break;
		}
		else
		{	
			while (i != str.end() && *i != '\n')
				i++;
		}
	}
	return s_line;
}

std::vector<t_locations> split_locations(std::string &str)
{
	std::string::iterator i;
	std::string::iterator end;
	std::vector<t_locations> loc;
	size_t index;
	t_locations all;
	int		len_of_one_server;
	i = str.begin();
	end = str.end();
	i += ft_skip_spases(i);
	while (i != end)
	{
		if (found_block("location ", i) == 1)
		{
			len_of_one_server = found_last_location(i, end);
			if (len_of_one_server == -1)
				ft_exit(NO_BRAKET);
			all.full_loc = (my_substr(i, len_of_one_server));
			loc.push_back(all);
			index = i - str.begin();
			str = my_substr(str.begin(), i) + my_substr(i + len_of_one_server, end);
			end = str.end();
			i = str.begin() + index;
		}
		else
			i++;
	}
	return loc;
}

unsigned long long set_size(std::string str)
{
	if (str.empty())
		return 0;
	std::string s = my_substr(str.begin() + 14, str.end());
	s = ft_trim_spases(s);
	unsigned long long i = strtoul(s.c_str(), NULL, 10);
	size_t n = 0;
	while (isnumber(s[n]))
		n++;
	
	switch (s[n])
	{
	case 'K':
		if (i > (ULONG_MAX >> 10))
			i = ULONG_MAX;
		else 
			i = i << 10;
		break;
	case 'M':
		if (i > (ULONG_MAX >> 20))
			i = ULONG_MAX;
		else 
			i = i << 20;
		break;
	case 'G':
		if (i > (ULONG_MAX >> 30))
			i = ULONG_MAX;
		else 
			i = i << 30;
		break;
	default:
		if (n == s.length())
			;
		else
			ft_exit(MAX_SIZE_ERROR);
		break;
	}
	
	return i;
}

void	ClassParser::pars_data()
{
	std::vector<std::string> lines_for_server = split_servers();
	size_t j = 0;
	while (j < lines_for_server.size())
	{
		this->data.push_back(pars_data_for_servers(lines_for_server[j]));
		j++;
	}
}

void	pars_check_max_body_size_loc(unsigned long &max_body_size, unsigned long server)
{
	if (max_body_size == 0)
		max_body_size = server;
}


void ClassParser::set_default_values()
{
	size_t i = 0;
	size_t j;
	
	if (data.empty())
		ft_exit(777);
	while (i < data.size())
	{
		pars_check_name(data[i].server_name);
		pars_check_ip(data[i].ip);
		pars_check_port(data[i].port);
		pars_check_root(data[i].root);
		pars_check_max_body_size(data[i].max_body_size);
		data[i].error_page = set_error_page_map(data[i].error_page_v, data[i].root);
		j = 0;
		if (data[i].location.empty())
			ft_exit(777);
		while (j < data[i].location.size())
		{	
			pars_check_max_body_size_loc(data[i].location[j].max_body_size, data[i].max_body_size);
			adress_cat(data[i].location[j].location, data[i].root);
			add_first_slash(data[i].location[j].location);
			add_last_slash(data[i].location[j].location);
			pars_check_location_root(data[i].location[j].root, data[i].root);
			pars_check_cgi(data[i].location[j].cgi_path, data[i].location[j].cgi_extensions);
			pars_check_auth(data[i].location[j]);
			j++;
		}
		i++;
	}
	check_ip_port_unique(data);
}