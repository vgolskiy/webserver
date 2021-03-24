/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readConfig.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mskinner <v.golskiy@ya.ru>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 11:16:22 by mskinner          #+#    #+#             */
/*   Updated: 2021/03/24 02:39:20 by mskinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

void	*ft_memset(void *dest, int c, size_t len)
{
	unsigned char	*dest_str;

	if (len == 0)
		return (dest);
	dest_str = (unsigned char *)dest;
	while (len--)
		*dest_str++ = (unsigned char)c;
	return (dest);
}

void	*ft_calloc(size_t nmemb, size_t size)
{
	unsigned char	*res;

	if (!(res = (unsigned char *)malloc(size * nmemb)))
		return (NULL);
	ft_memset(res, 0, nmemb * size);
	return ((void *)res);
}

int		ft_isspace(int c)
{
	if ((c == 32) || ((c >= 9) && (c <= 13)))
		return (1);
	return (0);
}

std::string	read_file(const char *file_path) {
	std::string		res;
	int				readed;
	int				fd;
	char			*buf;

	fd = open(file_path, O_RDONLY);
	if (errno)
		throw (errno);
	if (!(buf = (char *)ft_calloc(BUFFER_SIZE + 1, sizeof(char))))
		throw (-1);
	while ((readed = read(fd, buf, BUFFER_SIZE))> 0) {
		buf[readed] = '\0';
		res.append(buf);
	}
	if (readed < 0)
		throw (errno);
	free(buf);
	close(fd);
	return (res);
};

std::vector<std::string>	split_to_lines(const std::string &s, const std::string &delimiter) {
	std::vector<std::string>	res;
	size_t						pos;
	size_t						prev;

	prev = 0;
	while ((pos = s.find(delimiter, prev)) != std::string::npos) {
		res.push_back(s.substr(prev, pos - prev));
		prev = pos + 1;
	}
	// To get the last substring (or only, if delimiter is not found)
    res.push_back(s.substr(prev));
	return (res);
}

std::vector<std::string>	Config::parse_line(std::string line) {
	std::vector<std::string>	res;
	//positions of parameter description symbols in line
	size_t						begin;
	size_t						end;

	end = 0;
	while (end < line.length()) {
		begin = line.find_first_not_of(SPACES, end);
		if (begin == std::string::npos)
			return (res);
		end = line.find_first_of(SPACES, begin);
		if (end == std::string::npos)
			end = line.length();
		res.push_back(line.substr(begin, (end - begin)));
	}
	return (res);
};

void		Config::parse_configuration_file(std::vector<std::string> file_lines) {
	std::vector<std::string>::iterator	it;
	std::vector<std::string>			tokens;
	int									index;

	for (it = file_lines.begin(); it != file_lines.end(); ++it) {
		tokens = parse_line(*it);
		index++;
		if ((!tokens.size()) || (tokens[0][0] == 35))
			continue ;

		// parse_server_data(&tokens, index);
		
		printContainer(tokens);
		std::cout << std::endl;
	}
};

void	error_message(std::string message)
{
	std::cout << message << std::endl;
	//TODO: exit if parse size or unknown/double tokens - invalid (close fd(?) + exit(1));
};

//Если мы юзаем контейнер - массив, то там несколько серверов может быть, следовательно нужен индекс?
//Not tested
void	Config::parse_server_data(std::vector<std::string> &to_parse, int index)
{
	if (to_parse.size() == 1 || (to_parse.size() > 2 && to_parse[2][0] != '#'))
		error_message("Invalid arguments in config file!");
	if (to_parse[0] == HOST && _servers[index].host.size() == 0)
		_servers[index].host = to_parse[1];
	else if (to_parse[0] == NAME && _servers[index].name.size() == 0)
		_servers[index].name = to_parse[1];
	else if (to_parse[0] == PORT && _servers[index].port.size() == 0)
		_servers[index].port = to_parse[1];
	else if (to_parse[0] == ERR_PAGE && _servers[index].error_page.size() == 0)
		_servers[index].error_page = to_parse[1];
	else
		error_message("Unknown or double token!");
};
