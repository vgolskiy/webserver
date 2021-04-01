/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mskinner <v.golskiy@yandex.ru>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/31 17:35:07 by mskinner          #+#    #+#             */
/*   Updated: 2021/03/31 18:01:43 by mskinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#ifndef _CLIENT_HPP_
# define _CLIENT_HPP_

# include "Config.hpp"

extern t_config						g_config;

class Client
{
private:
	/* data */
public:
	Client(/* args */);
	~Client();

	void init_sockaddr_in(struct sockaddr_in &address, unsigned short port, std::string host);
};

#endif
