#include "Socket.hpp"

Socket::Socket(int port, std::string host)
{
    _port = port;
    _host = host;
    _opt = 1;
    init_socket();

    if((_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) // TODO: check arguments
    {
        std::cerr << strerror(errno) << "\n";
        exit(EXIT_FAILURE);
    }
}

Socket::~Socket(){}

void Socket::init_socket()
{
    _address.sin_family = AF_INET; // TODO: check if it's always like this?
    _address.sin_port = htons(_port); // TODO: htons already applied?
    if((_address.sin_addr.s_addr = inet_addr(_host.c_str())) <= 0) {
		std::cerr << "\nInvalid address/ Address not supported \n";
		exit(EXIT_FAILURE); // TODO: IS IT OK??????
    }
}

void Socket::to_bind()
{
    if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &_opt, sizeof(_opt)))
    {
        std::cerr << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
    }
    if (bind(_fd, (struct sockaddr*)&_address, sizeof(_address)) < 0) {
		std::cerr << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
    }
}

void Socket::to_listen(int num_ports)
{
    if (listen(_fd, num_ports) < 0)
    {
        std::cerr << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
    }
}
