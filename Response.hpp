#pragma once

/*
Структура Response:
1.Строки состояния HTTP ответа, в которой сервер указывает версию HTTP протокола и код состояния.
2.Нуля или нескольких полей HTTP заголовка, разделенных между собой символом CRLF.
3.Пустой строки (в этой строке должен быть только символ CRLF), эта строка обозначает окончание полей заголовка.
4.Необязательное тело HTTP сообщения.

1. Example:
Status-Line = HTTP-Version SP Status-Code SP Reason-Phrase CRLF

*/

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "Config.hpp"
# include "Request.hpp"
# include <dirent.h>

struct	t_location;
struct	t_server;
class	Client;
class	Response
{
private:
	std::string							_response;
	std::map<std::string, std::string>	_headers;
	Client*								_client;
	std::string							_method;
	std::string 						_body;
	int									_status_code;
	std::map<int, std::string>			_status;
	t_location*							_loc;
	std::string							_requested_index;
	std::map<std::string, std::vector<std::string> >	_content_types;
	std::list<std::string>				_headers_sequence;

public:
	Response(Client *client, t_server *server, std::string loc, std::string requested_index);
	~Response();

	void set_status();
	std::string get_server_date(void);
	std::string get_last_modified_date(std::string file);
	void		create_response(void);
	void		fill_response_body(void);
	std::string get_response_body(void);
	std::string	get_page_body(void);
	std::string	get_content_type(void);
	void		get_status_line(void);
};


#endif //WEBSERVER_RESPONSE_HPP
