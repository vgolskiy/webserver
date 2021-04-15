/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_test.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mskinner <v.golskiy@yandex.ru>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 23:17:50 by mskinner          #+#    #+#             */
/*   Updated: 2021/04/13 18:26:32 by mskinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** to compile run separately:
** clang++ -Wall -Werror -Wextra -std=c++98 client.cpp -o client
** clang++ -Wall -Werror -Wextra -std=c++98 server.cpp -o server
** to run program execute server then client in different terminals
*/

#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#define PORT 8000 
   
int main() { 
	int sock = 0;
	size_t	sent = 0;
	struct sockaddr_in serv_addr;
	const char* hello = "POST /cgi-bin/process.cgi HTTP/1.1\r\nUser-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\nHost: www.example.com\r\nContent-Type: application/x-www-form-urlencoded\r\nTransfer-Encoding: chunked\r\nAccept-Language: ru-ru\r\nAccept-Encoding: gzip, deflate\r\nConnection: Keep-Alive\r\n\r\n\r\n";
	char buffer[1024] = {0};

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		std::cerr << "\n Socket creation error \n";
		return (EXIT_FAILURE); 
    }

	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(PORT);

	// Convert IPv4 and IPv6 addresses from text to binary form 
	if((serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1"))<=0) {
		std::cerr << "\nInvalid address/ Address not supported \n";
		return (EXIT_FAILURE); 
    }

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		std::cerr << "\nConnection Failed \n";
		return (EXIT_FAILURE);
    }

	if ((sent = send(sock, hello, strlen(hello), 0)) != strlen(hello))
		std::cerr << "Not all msg sent\n";
	std::cout << "Hello message sent\n";
	read(sock, buffer, 1024);
	std::cout << buffer << std::endl;
	return (EXIT_SUCCESS); 
}
