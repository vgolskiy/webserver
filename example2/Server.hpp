//
// Created by Kanat Saribaew on 3/5/21.
//

#ifndef SERVER_HPP
#define SERVER_HPP

#include "Client.hpp"
#include "Socket.hpp"
#include "ClassParser.hpp"
#include "HttpHeaders.hpp"
#include "ClassGenErrorPage.hpp"
class Server {
private:
	HttpHeaders _generate_headers;
	ClassGenPage _gen_page;
	Socket _socket;
	int _socket_server;
	t_server_config_data _config;
	std::vector<Client> _client;
	void _controller(Client &, t_locations &, std::string&, std::pair<bool, size_t>);
	std::vector<std::string> _error_page();
	std::string _get_remote_addr(int fd, sockaddr_in *addr, socklen_t *len);
	std::pair<bool, std::pair<size_t, size_t> >  _checking_сorrectness_of_request(Http &http);
	void _execute_methods(Client &client);
	void _error_handler(Http &http, int cod_error, t_locations &loc, std::string l);
	std::string _get_extensions(const std::string &);
	std::vector<std::string> _get_method(Http &http);
	std::vector<std::string> _head_method(Http &http);
	std::vector<std::string> _put_method(Http &http, t_locations &locations);
	std::pair<bool, size_t> _check_authorization(Http &http, t_locations &loc);
	std::pair<bool, size_t> who_execute(t_locations &loc, std::string execute);
	std::pair<bool, size_t> _check_locations(const std::string &loc, const std::vector<t_locations> &locs);
	std::pair<bool, std::string>  _check_source(const std::string &location, const std::string &source, t_locations &);
	std::pair<bool, size_t> _check_methods(const std::string &method, const std::vector<std::string> &methods);

public:
	const std::vector<Client> &getClient() const;
	Server(const t_server_config_data &config);
	void new_connection();
	int getSocketServer() const;
	void recive(int index_client);
	void response(int index_client);
	void close_connect(int index_client);
};


#endif //SERVER_HPP
