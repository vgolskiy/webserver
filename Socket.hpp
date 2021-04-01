#pragma once

# include "Config.hpp"

class Config;

class Socket // add this class to struct service
{
    private:
        int                 _fd;
        int                 _port;
        int                 _opt;
        std::string         _host;
        struct sockaddr_in  _address;
    public:
        Socket(int port, std::string host);
        ~Socket();

        void init_socket();
        void to_bind();
        void to_listen(int num_ports);
};
