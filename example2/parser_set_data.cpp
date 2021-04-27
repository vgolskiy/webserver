/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_set_data.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbarbera <fbarbera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/08 17:07:07 by fbarbera          #+#    #+#             */
/*   Updated: 2021/03/21 16:00:25 by fbarbera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_config.hpp"

std::string::iterator	&check_error_token(std::string::iterator &s, std::string::iterator end)
{
	while (s != end)
	{
		if (ft_spases_p(*s) || *s == '{'|| *s == '}')
			s++;
		else
			return s;
	}
	return s;
}


static bool set_auto(std::string str)
{
	if (str.length() < 10)
		return (false);
	str = ft_trim_spases(my_substr(str.begin() + 10, str.end()));
	if (str == "on")
		return (true);
	else if (str == "off")
		return (false);
	ft_exit(100);
	return (false);
}

std::vector<std::string>	split_vector(std::string s, std::string type)
{
	std::vector<std::string> split;
	if (s.empty())
		return split;
	std::string str = my_substr(s.begin() + type.length(), s.end());
	std::string new_str;
	std::string::iterator i = str.begin();
	while (i != str.end())
	{
		if (ft_spases_p(*i))
			i++;
		else
		{
			std::string new_str;
			while (ft_spases_p(*i) == 0 && i != str.end())
			{
				new_str += *i;
				i++;
			}
			split.push_back(new_str);
		}
	}
	return (split);
	
}

std::string		only_check_path(std::string location)
{
	location = ft_trim_spases(location);
	if (location.empty())
		location = "/"; //так?
	if (location == ".")
		location = "/";
	double_sl(location);
	return location;
}

std::string		split_string(std::string s, std::string type)
{
	if (s.empty())
		return "";
	std::string str = my_substr(s.begin() + type.length(), s.end());
	return ft_trim_spases(str);
}

static std::string		set_port(std::string s)
{
	std::string::reverse_iterator i = s.rbegin();
	std::string::reverse_iterator end = s.rend();
	std::string port;
	while (i != end && ft_spases_p(*i))
		i++;
	while (i != end && isnumber(*i))
	{
		port+= *i;
		i++;
	}
	if (*i != ':')
		ft_exit(NO_PORT);
	if (port.empty())
		return "";
	std::string new_port;
	for(int n = port.length()-1; n >= 0; n--)
    	new_port.push_back(port[n]);
	return ft_trim_spases(new_port);
}

static std::string set_ip(std::string s)
{
	if (s.empty())
		exit(1);
	std::string::iterator i = s.begin();
	std::string ip;
	while (i != s.end() && *i != ':')
	{
		ip+=*i;
		i++;
	}
	if (i == s.end())
		ft_exit(IP_ERROR);
	return  ip;
}

static std::vector<std::string>	set_all_method()
{
	std::vector<std::string> a;
	a.push_back("GET");
	a.push_back("HEAD");
	a.push_back("POST");
	a.push_back("PUT");
	return a;
}

static bool	check_method_pars(std::vector<std::string> method, std::vector<std::string> all_method)
{
	bool a;
	for (size_t i = 0; i < method.size(); i++)
	{
		a = false;
		for (size_t j = 0; j < all_method.size(); j++)
			if (all_method[j] == method[i])
				a = true;
		if (a == false)
			return false;
	}
	return a;
}

std::string set_loc(std::string s)
{
	std::string str;
	str = my_substr(s.begin(), my_find(s,"{"));
	str = ft_trim_spases(str);
	return str;
}

std::map<int, std::string> set_error_page_map(std::vector<std::string> vector, std::string root)
{
	std::map<int, std::string> map;
	for (size_t i = 0; i < vector.size(); i++)
	{
		std::vector<std::string> v = ft_split_string_to_vector(vector[i], ' ');
		if (v.size() != 3 || !ft_isnumstring(v[1]))
			ft_exit(ERROR_PAGE_ERROR);
		int num = std::stoi(v[1]);
		if (num < 0)
			ft_exit(ERROR_PAGE_ERROR);
		adress_cat(v[2], root);
		std::pair<std::map<int, std::string>::iterator, bool> a;
		a = map.insert(std::pair<int, std::string>(num, v[2]));
		if (!a.second) //проверка на совпадения
			ft_exit(ERROR_PAGE_ERROR);
	}
	return map;
}

static void iset_data(t_server_config_data &s)
{
	if (s.server_name.empty())
		;
	else s.server_name = split_vector(s.server_name[0], "server_name ");
	s.root = (split_string(s.root, "root "));
	if (s.ip.empty())
		ft_exit(IP_ERROR);
	std::string copy = s.ip;
	s.ip = ft_trim_spases(set_ip(split_string(copy, "listen ")));
	s.port = set_port(copy);
	s.server_name.push_back(s.ip);
	s.all_method = set_all_method();
	size_t j = 0;
	while (j < s.location.size())
	{
		s.location[j].location = set_loc(split_string(s.location[j].location, "location "));
		if (s.location[j].root.empty())
			s.location[j].root = "";
		else
			s.location[j].root = split_string(s.location[j].root, "root ");
		if (s.location[j].method.empty())
			ft_exit(NO_METHOD);
		else
			s.location[j].method = split_vector(s.location[j].method[0], "method ");
		if (check_method_pars(s.location[j].method, s.all_method) == false)
			ft_exit(UN_METHOD);
		if (!s.location[j].cgi_path.empty())
			s.location[j].cgi_path = split_vector(s.location[j].cgi_path[0], "cgi_path ");

		if (!s.location[j].cgi_extensions.empty())
			s.location[j].cgi_extensions = split_vector(s.location[j].cgi_extensions[0], "cgi_extensions ");
		
		if (s.location[j].upload_storage.empty())
			s.location[j].upload_storage = "";
		else
			s.location[j].upload_storage = split_string(s.location[j].upload_storage, "upload_storage ");
		if (!s.location[j].path_to_auth.empty())
		{
			s.location[j].path_to_auth = split_string(s.location[j].path_to_auth, "auth_basic_user_file ");
			if (!s.location[j].path_to_auth.empty())
				s.location[j].auth = true;
		}
		if (!s.location[j].index_types.empty())
			s.location[j].index_types = split_vector(s.location[j].index_types[0], "index ");
		j++;
	}
}

t_server_config_data	pars_data_for_servers(std::string str)
{
	t_server_config_data s;
	std::string::iterator iterator;
	s.location = split_locations(str);
	size_t j = 0;
	while (j < s.location.size())
	{
		s.location[j].location = set_string_p(s.location[j].full_loc, "location ");
		s.location[j].root = set_string_p(s.location[j].full_loc, "root ");
		s.location[j].autoindex = set_auto(set_string_p(s.location[j].full_loc, "autoindex "));
		s.location[j].method = set_vector(s.location[j].full_loc, "method ");
		s.location[j].cgi_extensions = set_vector(s.location[j].full_loc, "cgi_extensions ");
		s.location[j].cgi_path = set_vector(s.location[j].full_loc, "cgi_path ");
		s.location[j].upload_storage = set_string_p(s.location[j].full_loc, "upload_storage ");
		s.location[j].path_to_auth = set_string_p(s.location[j].full_loc, "auth_basic_user_file ");
		s.location[j].index_types = set_vector(s.location[j].full_loc, "index ");
		s.location[j].max_body_size = set_size(set_string_p(s.location[j].full_loc, "max_body_size "));
		iterator = s.location[j].full_loc.begin() + 7;
		if (check_error_token(iterator, s.location[j].full_loc.end()) != s.location[j].full_loc.end())
			ft_exit(ERROR_TOKEN, iterator);
		j++;
	}
	s.server_name = set_vector(str, "server_name ");
	s.root = set_string_p(str, "root ");
	s.ip = set_string_p(str, "listen ");
	s.error_page_v = set_error_page(str, "error_page ");
	s.max_body_size = set_size(set_string_p(str, "max_body_size "));
	s.mime_map = gen_mime();
	iterator = str.begin() + 7;
	if (check_error_token(iterator, str.end()) != str.end())
		ft_exit(ERROR_TOKEN, iterator);
	iset_data(s);
	return s;
}