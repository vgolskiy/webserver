/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_test.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mskinner <v.golskiy@ya.ru>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 23:17:50 by mskinner          #+#    #+#             */
/*   Updated: 2021/04/20 20:14:58 by mskinner         ###   ########.fr       */
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
	const char* hello = "POST / HTTP/1.1\r\nAccept: */*\r\nAuthorization: Basic asdasdasdasd\r\nAccept-Encoding: gzip\r\nAccept-Language: uk,en-US;q=0.8,en;q=0.5,ru;q=0.3\r\nCache-Control: no-cache\r\nConnection: Keep-Alive\r\nHost: localhost:8080\r\nContent-Length: 16743\r\nUser-Agent: Mozilla/5.0 (Windows NT 10.0; WOW64; Trident/7.0; rv:11.0) like Gecko\r\ncontent-type: application/json\r\n\r\n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\r\n\r\n";
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
