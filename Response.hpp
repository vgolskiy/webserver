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

# ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "Config.hpp"
#include "Request.hpp"
#include <dirent.h>

class Client;
class Response
{
private:
	std::string							_response;
	std::map<std::string, std::string>	_headers;
	Client*								_client;
	std::string							_method;
	std::string 						_body;
	std::map<int, std::string>			_status;
public:
	Response(Client *client);
	~Response();

	void set_status();
	std::string server_date(void);
	std::string last_modified(std::string file);
	void		create_response(void);
	void		fill_response_body(void);
	std::string get_response_body(void);
};


#endif //WEBSERVER_RESPONSE_HPP
