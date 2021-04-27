/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClassGenErrorPage.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbarbera <fbarbera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/11 20:07:20 by fbarbera          #+#    #+#             */
/*   Updated: 2021/03/21 16:10:27 by fbarbera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClassGenErrorPage.hpp"

int ClassGenPage::found_error(int error, std::string &error_page)
{
	error_page = data[error];
	if (error_page.empty())
		return 0;
	std::string line;
    std::ifstream in(error_page); // окрываем файл для чтения
	error_page.clear();
    if (in.is_open())
    {
        while (getline(in, line))
			error_page += line + "\n";
    }
	else
		return 0;
    in.close();     // закрываем файл
	return 1;
}




std::string ClassGenPage::gen_error_page(int error)
{
	std::string error_page;
	
	if (found_error(error, error_page) == 1)
		return error_page;
	return (s1 + std::to_string(error) + s2);
}

std::string ClassGenPage::gen_error_page(std::string error)
{
	return (s1 + error + s2);
}

ClassGenPage::ClassGenPage(std::map<int, std::string> data)
{
	this->data = data;
	s1 = "<html><head><title>ERROR PAGE</title></head><body><br /><br /><br /><p align=\"center\" style=\"color:#ff4444;font-size:80px\"> <b>ERROR ";
	s2 = ".</b></p></body></html>";
}

ClassGenPage::~ClassGenPage()
{
}


int ClassGenPage::count_dir(std::string str)
{
	int num = 0;
	for (size_t i = 0; i < str.length(); i++)
		if (str[i] == '/')
			num++;
	return num;
}

std::string ClassGenPage::gen_auto_page(std::string path, std::string location)
{
	std::string head1 = "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=windows-1252\"><title>Index of ";
	std::string head2 = "</title></head><body bgcolor=\"white\"><h1>Index of ";
	std::string head3 = "</h1><hr><pre>";
	std::string head = head1 + location + head2 + location + head3;
	std::string tail = "</pre><hr></body></html>";
	std::string body;
	DIR *dir;
	struct dirent	*entry;
	std::string full_path = path;
	add_first_slash(location);
	add_last_slash(location);
	dir = opendir(path.c_str());
	while (dir && (entry = readdir(dir)) != nullptr)
	{
		if (std::string(entry->d_name) == ".")
			continue ;
		int n = body.length();
		if (std::string(entry->d_name) == ".." && count_dir(location) == 1)
			body += "<a href=\"/\">../</a>";
		else
		{
			if (entry->d_type == DT_DIR)
				body += "<a href=\"" + std::string(entry->d_name) + "/\">" + std::string(entry->d_name) + "/</a>";
			else	
				body += "<a href=\"" + std::string(entry->d_name) + "\">" + std::string(entry->d_name) + "</a>";
		}
		// _____trash?
		int sp = body.length() - n - std::string(entry->d_name).length();
		while (sp < 80)
		{
			body += " ";
			sp++;
		}
		if (entry->d_type == DT_DIR)
			body+=" -\n";
		else
			body+="0\n";
		// _____trash?
		
	}
	if (dir){
		closedir(dir);
	}
	return head+body+tail;
}
