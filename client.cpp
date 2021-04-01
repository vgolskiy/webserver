/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mskinner <v.golskiy@yandex.ru>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/31 18:01:21 by mskinner          #+#    #+#             */
/*   Updated: 2021/03/31 18:01:43 by mskinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client() {}

Client::~Client() {}

void    Client::init_sockaddr_in(struct sockaddr_in &address, unsigned short port, std::string host)
{
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = inet_addr(host.c_str());
}
