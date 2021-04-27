/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_config.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbarbera <login@student.21-school.ru>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/02 15:07:29 by fbarbera          #+#    #+#             */
/*   Updated: 2021/03/20 22:47:22 by fbarbera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_CONFIG_HPP
# define PARSER_CONFIG_HPP

# include "parser_errors.hpp"
# include "ClassGenErrorPage.hpp"
# include <iostream>
# include <vector>
# include <list>
# include <map>
# include <string>
# include <fstream>


typedef struct s_auth
{
	std::string	AuthType;
	std::string login;
	std::string password;
	std::string password_64;
}				t_auth;

typedef struct	s_locations
{
	std::string					full_loc; //мусорная строка для отлова лишнего
	std::string					location; //+
	std::string					root; //корневая папка локейшена (если нет в клнфиге заменяю на дефолтный серверный рут)
	unsigned long				max_body_size; // заменяю на серверный (если нет в кофиге) // удалить
	bool						autoindex; //+ (по дефолту off)
	std::vector<std::string>	method; //+ (если нет методов/не сущетвующий - выдаю ошибку)
	std::vector<std::string>	cgi_extensions; // если нет, то (не знаю???? )
    std::vector<std::string>	cgi_path; //+ если пусто то cgi не поддерживается
    std::string					upload_storage; //+ путь куда исполнить медот PUT (если нет метода пут - ошибка?)
	std::string					path_to_auth;
	std::vector<std::string>	index_types; //+
	t_auth						auth_data;
	bool						auth; 
}				t_locations;

typedef struct	s_server_config_data
{
	std::vector<t_locations>	location;
	std::vector<std::string>	server_name; //имя сервера + ip
	std::string					ip; //+
	std::string					port; //+
    std::string					root; //+ по дефлту .
    std::vector<std::string>	error_page_v; //- 
	std::map<int, std::string>	error_page; 
    unsigned long				max_body_size; // - не указано (по дефолту 1Gb)
	std::vector<std::string>	all_method; // + GET HEAD POST PUT // для каната
	std::map<std::string, std::string> mime_map;

}				t_server_config_data;

int						ft_skip_spases(std::string::iterator i);
std::string				delete_comments(std::string line);
int						found_block(std::string str, std::string::iterator i);
int						ft_spases_p(char c);
void					ft_error_p();
std::string				my_substr(std::string::iterator i, std::string::iterator end);
int 					found_last_newline(std::string::iterator i, std::string::iterator end);
std::string				my_substr(std::string::iterator i, int len);
int						found_last_location(std::string::iterator i, std::string::iterator end);
int						found_last_bracket(std::string::iterator i, std::string::iterator end);
std::string				ft_trim_spases(std::string s);
std::string::iterator	my_find(std::string &s, std::string des);
std::string 			set_string_p(std::string &str, std::string found);
std::string 			base64_decode(std::string const& encoded_string);
std::vector<t_locations> split_locations(std::string &str);
unsigned long long 		set_size(std::string str);
std::vector<std::string> set_vector(std::string &str, std::string found);
t_server_config_data	pars_data_for_servers(std::string str);
std::vector<std::string> set_error_page(std::string &str, std::string found);
void					pars_check_name(std::vector<std::string> server_name);
void					pars_check_ip(std::string ip);
void					pars_check_port(std::string port);
void					pars_check_root(std::string &root);
void					pars_check_max_body_size(unsigned long &size);
void					adress_cat(std::string &location, std::string root);
void					pars_check_location_root(std::string &root, std::string server_root);
void					pars_check_location_max_body_size(unsigned long &max_body_size, unsigned long server_max_body_size);
std::vector<std::string> ft_split_string_to_vector(std::string str, char ch);
bool					ft_isnumstring(std::string str);
void					pars_check_cgi(std::vector<std::string> &cgi_path, std::vector<std::string> cgi_extensions);
void					add_last_slash(std::string &str);
std::map<std::string, std::string> gen_mime();
void					add_first_slash(std::string &str);
std::map<int, std::string> set_error_page_map(std::vector<std::string> vector, std::string root);
std::map<std::string, std::string> gen_http_code();
void	pars_check_auth(s_locations &location);
void	check_ip_port_unique(std::vector<s_server_config_data> data);
void  double_sl(std::string root);
#endif