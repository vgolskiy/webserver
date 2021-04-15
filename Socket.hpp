#ifndef SOCKET_HPP
#define SOCKET_HPP

# include "Config.hpp"

class Config;

class Socket // add this class to struct service
{
	private:
		int					_fd;
		unsigned short		_port;
		int					_opt;
		std::string			_host;
		struct sockaddr_in	_address;
		Socket(void);

	public:
		Socket(int port, std::string host);
		Socket(const Socket &copy);
		~Socket(void);
		Socket	operator=(const Socket &other);

		void	init_socket();
		void	to_bind();
		void	to_listen(int num_ports);
		int		get_fd(void) const;
};

#endif
