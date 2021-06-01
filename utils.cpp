/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mskinner <v.golskiy@ya.ru>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/15 19:28:29 by mskinner          #+#    #+#             */
/*   Updated: 2021/06/01 22:15:09 by mskinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

// Если процесс пытается записать данные в оборванный сокет при помощи вы­зова write или send,
// то он получит сигнал SIGPIPE, который может быть пере­хвачен соответствующим обработчиком сигнала
// при вызове read или write может возникнуть блокирование:

// закрывается дескриптор файла, открытого только на чтение:
// ядро посылает всем процессам, ожидающим записи в канал, сигнал SIGPIPE
// write -> return (-1);
// errno(EPIPE);
void		signals(void) {
	signal(SIGPIPE, SIG_IGN);
	signal(SIGINT, signal_handler);
}

int			got_signal(void) {
	static int signal = 0;
	return (signal);
}

void		signal_handler(int signal)
{
	if (signal == SIGINT) {
		clear_servers_configuration();
		exit(EXIT_SUCCESS);
	}
}

void		exit_error(int err) {
	error_message(strerror(err));
	clear_servers_configuration();
	exit(EXIT_FAILURE);
}

void		exit_error_msg(std::string msg) {
	error_message(msg);
	clear_servers_configuration();
	exit(EXIT_FAILURE);
}

long		current_time() {
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

std::string	tail(const std::string &s, const size_t length) {
	if (length >= s.size())
		return (s);
	return (s.substr(s.size() - length));
}

std::string	inet_ntoaddr(int n) {
	std::string res = std::to_string((n / int(std::pow(256, 0))) % 256) + '.';
	res += std::to_string((n / int(std::pow(256, 1))) % 256) + '.';
	res += std::to_string((n / int(std::pow(256, 2))) % 256) + '.';
	res += std::to_string((n / int(std::pow(256, 3))) % 256);
    return (res);
}

void	error_message(std::string message)
{
	std::cerr << message << std::endl;
};

std::string	base64_encode(const std::string &s) {
	std::string::const_iterator	it;
	std::string					res;
	int	val = 0;
	int	valb = -6;

	for (it = s.begin(); it != s.end(); ++it) {
		val = (val << 8) + (unsigned char)(*it);
		valb += 8;
		while (valb >= 0) {
			res.push_back(BASE64[(val >> valb) & 0x3F]);
			valb -= 6;
		}
	}
	if (valb > -6)
		res.push_back(BASE64[((val << 8) >> (valb+8)) & 0x3F]);
	while (res.size() % 4)
		res.push_back('=');
    return (res);
}

std::string	base64_decode(const std::string &s) {
	std::string 				base = BASE64;
	std::string::const_iterator	it;
    std::string					res;
    std::vector<int>			code(256, -1);
	int	val = 0;
	int	valb = -8;

	//decoding mask for base64
    for (int i = 0; i < 64; ++i)
		code[BASE64[i]] = i;
    for (it = s.begin(); it != s.end(); ++it) {
		if ((base.find(*it) != std::string::npos) || (*it == '=')) {
        	if (code[*it] == -1)
				break;
        	val = (val << 6) + code[*it];
        	valb += 6;
        	if (valb >= 0) {
            	res.push_back(char((val >> valb) & 0xFF));
            	valb -= 8;
			}
        }
		else
			throw (std::string("Input is not a valid base64-encoded data"));
    }
    return (res);
}

bool is_hex(const std::string &hex) {
	const char	hexs[] = "0123456789abcdefABCDEF";
	int			hex_len = hex.length();

	if (!hex_len)
		return (false);
	for (int i = 0; i < hex_len; ++i) {
		//compare with each hex
		for (int j = 0; j < 22; ++j) {
			if (hex[i] == hexs[j])
				break;
			if (j == 21)
				return (false);
		}
	}
	return (true);
}

void to_insert(std::string str)
{
	std::string 		file; // define file
    std::string			res = "";
	std::stringstream 	ss;
	
    std::ifstream		inf(file);
	if (!inf)
		throw std::runtime_error(file); // add try
    ss << inf.rdbuf();
    res += ss.str();
    res += "\n";
    res += str;

    std::ofstream 		outf(file);
	if (!outf)
		throw std::runtime_error(file); // add try
    outf << res;

}
