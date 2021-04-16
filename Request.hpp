/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mskinner <v.golskiy@ya.ru>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/15 19:29:09 by mskinner          #+#    #+#             */
/*   Updated: 2021/04/16 19:20:19 by mskinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// http - сообщение 

/*

1.Начальная строка сообщения. (Метод)
2.Заголовок, который может содержать одно или несколько полей заголовка.
3.Пустая строка (на самом деле эта строка не пустая, она содержит символ CTRLF), которая обозначает конец заголовка.
4.Необязательное тело сообщения или HTTP объект.

*/

/*
Headers (FOR REQUESTS only):
◦ Accept-Charsets (какие наборы символов приемлемы для ответов сервера): utf-8
◦ Accept-Language (указывает серверу приемлемые языки): ru
◦ Authorization (отправки данных авторизации на сервер): Basic QWxhZGRpbjpvcGVuIHNlc2FtZQ==
◦ Content-Language: en, ru
◦ Content-Length (bytes): 1367
◦ Content-Location (альтернативное расположение содержимого сущности): 
◦ Content-Type: text/html;charset=utf-8 (не допускают пробелов вокруг символа равенства «=»)
				- Значение параметра, соответствующее производству токена, может быть передано либо в виде токена,
				- либо внутри строки в кавычках. Значения в кавычках и без кавычек эквивалентны.

◦ Date: Tue, 15 Nov 1994 08:13:45 GMT
◦ Host: ru.wikioedia.org
◦ Last-Modified (дата последней модификаци): 
◦ Referer (URI, после которого клиент сделал текущий запрос): http://en.wikipedia.org/wiki/Main_Page
◦ Transfer-Encoding (список способов кодирования): chunked
◦ User-Agent (список названий и версий клиента и его компонентов с комментариями):
*/

/*
Примеры запросов:

GET /hello.htm HTTP/1.1     (этот запрос посылает клиент)
 
Любое поле заголовка в HTTP сообщение имеет единый синтаксис:
имя_заголовка : значение

!!!!!HTTP сообщения не должны одновременно включать и поле заголовка Content-Length и применять кодирование передачи типа «chunked»!!!!!!!
Если поступило HTTP сообщение с полем Content-Length и закодированное с применением кодирования передачи «chunked», то поле Content-Length должно игнорироваться.


Если Request-URI — это absoluteURI, то хост — это часть Request-URI. Любое значение поля заголовка Host в запросе ДОЛЖНО игнорироваться (напомню про требования HTTP).
Если Request-URI — не absoluteURI, а запрос содержит поле заголовка Host, то хост определяется значением поля заголовка Host.
Если хоста, определенного правилами 1 или 2 не существует на сервере, код состояния ответа должен быть 400 (Испорченный Запрос, Bad Request).

*/

/*
Request =
	Request-Line ;
	*(( general-header ;
	request-header ;
	entity-header ) CRLF) ;
	CRLF [ message-body ] ; 
*/

/*
|CGI module|

CGI defines the abstract parameters,
known as metavariables, which
describe the client's request.

GET:
1.Получить значение переменной QUERY_STRING
2.Декодировать имена и их значения (учитывая, что все пробелы при декодировании сервером были 
  заменены символом "+" и все символы с десятичным кодом больше 128 преобразованы в символ "%" и следующим за ним шестнадцатеричным кодом символа.)
3.Сформировать структуру соответствия "имя - значение" для дальнейшего использования в cgi-модуле

POST:
1.Получить из стандартного входного потока CONTENT_LENGTH символов
2.Декодировать имена и их значения (учитывая, что все пробелы при декодировании сервером были 
  заменены символом "+" и все символы с десятичным кодом больше 128 преобразованы в символ "%" 
  и следующим за ним шестнадцатеричным кодом символа.)
3.Сформировать структуру соответствия "имя - значение" для дальнейшего использования в cgi-модуле

*/

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "Config.hpp"

# define HTTP "HTTP/1.1"

class Client;
class Request
{
private:
	std::string							_query_str;
	std::string							_method;
	std::string							_uri;
	std::string							_version;
	std::map<std::string, std::string>	_headers;
	std::string 						_body;
	std::vector<std::string>			_env;

	static std::string const			methods[];
	static std::string const			headers[];

	Client*								_client;
	int									_status;
	int									_remain_len; // bytes left to read
	int									_content_len;
	bool								_chunk;
public:
	Request(Client *client);
	~Request();

	void		parse_init(std::vector<std::string> &split_lines, std::string &orig_lines);
	void		parse_request(std::string &lines);
	bool		parse_chunk_size(std::string &lines);
	bool		parse_chunk_data(std::string &lines);
	bool		check_start_line(const std::vector<std::string> &lines);
	bool		set_up_headers(const std::vector<std::string> &lines);	
	bool		check_hex_chunk(std::string &to_check);
	void		set_cgi_meta_vars();
	void		cut_remain_len(int to_cut);
	std::string	find_header(std::string header);
	void		createResponce();
	int			get_remain_len();
	int 		get_status();
	int			get_content_length();
	void		print_parsed_request();
	std::vector<std::string> get_env();

	enum status
	{
		INIT,
		BODY_PARSE,
		HEADERS,
		C_G_I,
		MTH,
		CHUNK,
		CHUNK_DATA,
		BAD_REQ,
		DONE
	};
};

#endif
