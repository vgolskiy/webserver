/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_config.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbarbera <fbarbera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/02 14:40:54 by fbarbera          #+#    #+#             */
/*   Updated: 2021/03/18 21:49:55 by fbarbera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClassParser.hpp"

std::vector<t_server_config_data>		parser_config(void)
{
	ClassParser	p;

	p.fing_config_file(); // поиск конфига
	p.read_from_file(); // чтение конфига
	p.pars_data(); // разбитие на токены
	p.set_default_values(); // установка значений (дефолтные/по серверу)
	return (p.get_data());
}
 
 #include <sys/time.h>


#include "test.hpp"

int main()
{
	srand(time(nullptr));
	test a;

	std::string line;
	line = "3e8\r\nrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr\r\n0\r\n\r\n";
    std::string str;
	std::ifstream in("test_chan.txt"); // окрываем файл для чтения
    // if (in.is_open())
    // {
        while (a._status != "execute")
		{
			int i = rand() % line.length();
			str = line.substr(0, i);
			line.erase(0, i);
			a._buffer+=str;
			a._body_chunked();
			// std::cout << "body " << a._body << std::endl;
			// std::cout << a._status << std::endl;
			// 			a._body_chunked();
			// std::cout << "body " << a._body << std::endl;
			// std::cout << a._status << std::endl;
		}



	// for (int i = 0; i < 1000; i++)
	// 	std::cout << "r";





	
    // }
	// else
	// {
	// 	//std::cout << "error open" << std::endl; //error
	// 	exit(1);
	// }
    // in.close();     // закрываем файл
	
	
	
	
	
	
	
	
	
	// std::map<std::string, std::string> m;
	// m = gen_http_code();

	// 	for(std::map<std::string, std::string>::iterator it = m.begin(); it != m.end(); ++it)
	// 	{
	// 		std::cout << it->first << " : " << it->second;///вывод на экран
	// 	}
	
	// std::vector<t_server_config_data> p = parser_config();

	// std::cout << p[0].location[0].auth_data.AuthType << std::endl;
	// std::cout << p[0].location[0].auth_data.login << std::endl;
	// std::cout << p[0].location[0].auth_data.password << std::endl;
	// std::cout << p[0].location[0].auth_data.password_64 << std::endl;
	// std::cout << Get_local_time() << std::endl;

	
	// ClassGenPage er(p[0].error_page);
	// std::cout << er.gen_error_page(404);
	// std::cout << er.gen_auto_page(p[0].root, "/");

	// int i = 0;
	// int j;
	// int k;	
	// while (i < p.size())
	// {
	// 	j = 0;
	// 	std::cout << "_________serv___________"  << std::endl;
	// 	for (k = 0; k < p[i].server_name.size(); k++)
	// 		std::cout << p[i].server_name[k] << "\t\t\t server_name" << std::endl;
	// 	std::cout << p[i].ip << "\t\t\t ip" << std::endl;
	// 	std::cout << p[i].port << "\t\t\t\t port"<< std::endl;
	// 	std::cout << p[i].root << "\t\t\t root" << std::endl;
	// 	std::cout << p[i].max_body_size << "\t\t max_size"<< std::endl;
	// 	for(std::map<int, std::string>::iterator it = p[i].error_page.begin(); it != p[i].error_page.end(); ++it)
	// 	{
	// 		std::cout << it->first << " : " << it->second << std::endl;///вывод на экран
	// 	}
	// 	while (j < p[i].location.size())
	// 	{	
	// 		std::cout << "_________loc___________" << j  << std::endl;
	// 		std::cout << p[i].location[j].location << "\t\t\t location" << std::endl;
	// 		std::cout << p[i].location[j].root << "\t\t\t root" << std::endl;
	// 		std::cout << p[i].location[j].autoindex << "\t\t\t autoindex"<< std::endl;
	// 		for (k = 0; k < p[i].location[j].index_types.size(); k++)
	// 			std::cout << p[i].location[j].index_types[k] << "\t\t\t index" << std::endl;
	// 		for (k = 0; k < p[i].location[j].method.size(); k++) 
	// 			std::cout << p[i].location[j].method[k] << "\t\t\t method "<< std::endl;
	// 		for (k = 0; k < p[i].location[j].cgi_extensions.size(); k++)
	// 			std::cout << p[i].location[j].cgi_path[k] << " : " << p[i].location[j].cgi_extensions[k] << "\t\t cji"<< std::endl;
	// 		std::cout << p[i].location[j].upload_storage << "\t\t\t storage"<< std::endl;
	// 		j++;
	// 	}
	// 	i++;
	// }

	
	
//	return 0;
//}