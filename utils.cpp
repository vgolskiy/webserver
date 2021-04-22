/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mskinner <v.golskiy@ya.ru>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/15 19:28:29 by mskinner          #+#    #+#             */
/*   Updated: 2021/04/22 10:41:33 by mskinner         ###   ########.fr       */
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
