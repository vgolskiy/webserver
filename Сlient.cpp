/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Сlient.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mskinner <v.golskiy@yandex.ru>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/31 18:01:21 by mskinner          #+#    #+#             */
/*   Updated: 2021/03/31 18:01:43 by mskinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(Socket *listen_sock) : _listen_sock(listen_sock) {}

Client::~Client() {}


