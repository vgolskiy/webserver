/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mskinner <v.golskiy@ya.ru>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 17:01:17 by mskinner          #+#    #+#             */
/*   Updated: 2021/03/23 11:53:27 by mskinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <iostream>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string>
#include <errno.h>
#define PORT 8080 

int main() { 
	int server_fd, new_socket;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[1024] = {0};
	const char* hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";

    /*
	** Creating socket file descriptor
	** For TCP/IP sockets, we want to specify the IP address family (AF_INET)
	** and virtual circuit service (SOCK_STREAM). Since there’s only one form
	** of virtual circuit service, there are no variations of the protocol,
	** so the last argument, protocol, is zero.
	*/
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		std::cerr << strerror(errno) << std::endl;
		exit(EXIT_FAILURE); 
	}

    // Forcefully attaching socket to the port 8080 
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR , &opt, sizeof(opt))) {
		std::cerr << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	//The address family used during set up the socket
	address.sin_family = AF_INET;

	/*
	** The address for this socket or machine’s IP address.
	** With IP, machine will have one IP address for each network interface.
	** For example, if your machine has both Wi-Fi and ethernet connections - two addresses, one for each interface.
	** The special address for OS IP selection is 0.0.0.0, defined by the symbolic constant INADDR_ANY.
	*/
	address.sin_addr.s_addr = INADDR_ANY;

	//Server picks a specific port number since clients need to know a port number to connect to.
	address.sin_port = htons(PORT);
   
    /*
	** Forcefully attaching socket to the port 8080
	** Binding an address - assigning a transport address to the socket (a port number in IP networking)
	** sockaddr is a generic container that allows the OS to be able to read the first couple of bytes
	** that identify the address family, which determines what variant of the sockaddr struct to use
	** that contains elements that make sense for that specific communication type. 
	** For IP networking, struct sockaddr_in used, which is defined in the header netinet/in.h
	*/
	if (bind(server_fd, (struct sockaddr*)&address, sizeof(address))<0) {
		std::cerr << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
    }

	/*
	** Listen tells a socket that it should be capable of accepting incoming connections
	** The second parameter, backlog, defines the maximum number of pending connections
	** that can be queued up before connections are refused.
	*/
    if (listen(server_fd, 3) < 0) {
		std::cerr << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
    }

	/*
	** Accept grabs the first connection request on the queue of pending connections (set up in listen)
	** and creates a new socket for that connection.
	*/
	if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen))<0) {
		std::cerr << strerror(errno) << std::endl;
		exit(EXIT_FAILURE); 
	}

	read(new_socket, buffer, 1024);
	std::cout << buffer << std::endl;
	send(new_socket, hello, strlen(hello), 0);
	std::cout << "Hello message sent\n";
	close(new_socket);
	return (EXIT_SUCCESS);
}
