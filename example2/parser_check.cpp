/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_check.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbarbera <fbarbera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/08 18:32:18 by fbarbera          #+#    #+#             */
/*   Updated: 2021/03/21 15:59:01 by fbarbera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_config.hpp"

void	pars_check_name(std::vector<std::string> server_name)
{
	if (server_name.empty())
		ft_exit(NO_NAME);
}

static bool pars_check_ip_sim(std::string ip)
{
	for (size_t i = 0; i < ip.length(); i++)
	{
		if (ip[i] == '.' || isnumber(ip[i]))
			continue;
		else
			return true;
	}
	return false;
}
void	pars_check_ip(std::string ip)
{
	size_t  i = 0;
	std::string num_str;
	if (ip.length() > 16 || pars_check_ip_sim(ip))
		ft_exit(IP_ERROR);
	int number[4];
	for (int j = 0; j < 4; j++)
	{
		while (i < ip.length() && ip[i] != '.')
		{
			num_str+=ip[i];
			i++;
		}
		if (num_str.length() < 1)
			ft_exit(IP_ERROR);
		number[j] = std::stoi(num_str);
		if (number[j] >= 256 || number[j] < 0)
			ft_exit(IP_ERROR);
		i++;
		num_str.clear();
	}
}

void	pars_check_port(std::string port)
{
	if (port.empty() || port.length() > 6 || std::stoi(port) > 65535)
		ft_exit(NO_PORT);
}

void  double_sl(std::string root)
{
	int count = 0;
	for (size_t i = 0; i < root.length(); i++)
	{
		if (count != 1)
			count = 0;
		if (root[i] == '/')
			count++;
		else
			count--;
		if (count == 2)
			ft_exit(DOUBLE_SL);
	}
}

void	pars_check_root(std::string &root)
{
	if (root.empty()) 
		ft_exit(NO_ROOT);
	if (root == ".")
		root = "";
	if (*root.rbegin() == '/')
		root = root.substr(0, root.size()-1);
	double_sl(root);
}

void	pars_check_max_body_size(unsigned long &size)
{
	if (size == 0)
		size = (1 << 30);
}

std::string adresscat(std::string root, std::string location)
{
	std::string str;
	str = root;
	if (root.empty())
		return location;
	if (*str.rbegin() == '/' && *location.begin() == '/')
		str+=my_substr(location.begin() + 1, location.end());
	else if (*str.rbegin() != '/' && *location.begin() != '/')
		str += ('/' + location);
	else
		str += location;
	return str;
}

void	adress_cat(std::string &location, std::string root)
{
	if (location.empty())
		location = "/"; //так?
	if (location == ".")
		location = "/";
	double_sl(location);
	location = adresscat(root, location);
}

void pars_check_location_root(std::string &root, std::string server_root)
{
	if (!root.empty() && root == ".")
		root = "/";
	if (root.empty())
		root = server_root;
	add_last_slash(root);
	double_sl(root);
}

void	pars_check_location_max_body_size(unsigned long &max_body_size, unsigned long server_max_body_size)
{
	if (max_body_size == 0)
		max_body_size = server_max_body_size;
}

void	pars_check_cgi(std::vector<std::string> &path, std::vector<std::string> extensions)
{
	if (path.size() != extensions.size())
		ft_exit(PATH_EXTEN);
	for (size_t i = 0; i < extensions.size(); i++)
	{
		if (extensions[i].length() < 2 || extensions[i][0] != '.')
			ft_exit(EXTENTION_DOT);
	}
}

static const std::string base64_chars =
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";

static inline bool is_base64(unsigned char c) {
  return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string base64_decode(std::string const& encoded_string)
{
	int in_len = encoded_string.size();
	int i = 0;
	int j = 0;
	int in_ = 0;
	unsigned char char_array_4[4], char_array_3[3];
	std::string ret;

	while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
	char_array_4[i++] = encoded_string[in_]; in_++;
	if (i ==4) {
		for (i = 0; i <4; i++)
		char_array_4[i] = base64_chars.find(char_array_4[i]);

		char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
		char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
		char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

		for (i = 0; (i < 3); i++)
		ret += char_array_3[i];
		i = 0;
	}
	}

	if (i) {
	for (j = i; j <4; j++)
		char_array_4[j] = 0;

	for (j = 0; j <4; j++)
		char_array_4[j] = base64_chars.find(char_array_4[j]);

	char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
	char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
	char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

	for (j = 0; (j < i - 1); j++)
		ret += char_array_3[j];
	}
	return ret;
}

void	pars_check_auth(s_locations &location)
{
	if (!location.auth)
		return ;
	std::string line;
	std::string::iterator i;
    std::ifstream in(location.path_to_auth);
    if (in.is_open())
    {
        while (getline(in, line))
		{
			if ((i = my_find(line, "AuthType "))!= line.end())
				location.auth_data.AuthType = my_substr(i + 9, line.end());
			else if ((i = my_find(line, ":"))!= line.end())
				location.auth_data.login = line;
			else
				ft_exit(NO_VALID_AUTH_FILE);
		}
    }
	else
		ft_exit(AUTH_PATH, location.path_to_auth);
	location.auth_data.password = location.auth_data.login.substr(location.auth_data.login.find(':') + 1, location.auth_data.login.length());
    location.auth_data.login = my_substr(location.auth_data.login.begin(), my_find(location.auth_data.login, ":"));
	if (location.auth_data.password.empty() || location.auth_data.login.empty() || location.auth_data.AuthType != "Basic")
		ft_exit(NO_VALID_AUTH_FILE);
	location.auth_data.password_64 = location.auth_data.password;
	location.auth_data.password = base64_decode(location.auth_data.password);
	in.close();
	
}

void	check_ip_port_unique(std::vector<s_server_config_data> data)
{
	std::vector<std::string> v;
	for (size_t i = 0; i < data.size(); i++)
		v.push_back(data[i].ip + ":" + data[i].port);
	for (size_t i = 0; i < v.size(); i++)
	{
		for (size_t j = i + 1; j < v.size(); j++)
		{
			if (v[i] == v[j])
				ft_exit(UNICUE_IP);
		}
	}
}

// void	pars_check_max_body_size(data[i].max_body_size);
// void	adress_cat(data[i].location[j].location);
// void	pars_check_location_root(data[i].location[j].root, data[i].root);
// void	pars_check_location_max_body_size(data[i].location[j].max_body_size, data[i].max_body_size);
// void	pars_check_location_cgi(data[i].location[i]);
// void	pars_check_location_index(data[i].location[j].index_types, data[i].index_types);
