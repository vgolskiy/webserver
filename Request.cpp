/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mskinner <v.golskiy@ya.ru>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/15 19:29:16 by mskinner          #+#    #+#             */
/*   Updated: 2021/04/15 19:30:01 by mskinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request(Client *client) 
{
    _method = "";
    _query_str = "";
    _version = "";
    _uri = "";
    _body = "";
    _client = client;
    _remain_len = 0;
    _status = INIT;
    _content_len = -1;
    _chunk = false;
}

Request::~Request() {}

std::string const Request::methods[] = {
	"GET",
	"HEAD",
	"POST",
	"PUT",
};

std::string const Request::headers[] = {
	"Accept-Charsets",
	"Accept-Language",
	"Authorization",
	"Content-Language",
	"Content-Length",
	"Content-Location",
	"Content-Type",
	"Date",
	"Host",
	"Last-Modified",
	"Referer",
	"Transfer-Encoding",
	"User-Agent",
};

int Request::get_remain_len(){return _remain_len;}

int	Request::get_content_length() {return _content_len;}

int Request::get_status(){return _status;}

void Request::cut_remain_len(int to_cut){_remain_len -= to_cut;}

std::vector<std::string> Request::get_env(){return _env;}

void remove_spaces(std::string &str)
{
    char* chr = const_cast<char*>(str.c_str());
    int i = 0;

    while (chr[i] == ' ')
        i++;
    str = std::string(&chr[i]);
}

void Request::set_up_headers(const std::vector<std::string> &lines)
{
    std::vector<std::string>    tmp;
    bool                        flag;

    for (size_t i = 1; i < lines.size(); i++)
    {
        flag = false;
        tmp = split(lines[i], ":"); // Special case for localhost:XXXX
        for (size_t i = 0; i < tmp.size(); i++)
            remove_spaces(tmp[i]);
        for (size_t j = 0; j != headers->size(); j++)
        {
            if (tmp[0] == headers[j])
            {
                _headers[headers[j]] = tmp[1];
                flag = true;
            }
        }
        if (tmp[0] == "Content-Length")
        {
            try
            {
                _content_len = std::stoi(tmp[0]);
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }
        }
        if (tmp[0] == "Transfer-Encoding" && tmp[1] == "chunked")
        {
            _chunk = true;
            _content_len = -1;
        }
        tmp.clear();
    }
}

bool Request::check_start_line(const std::vector<std::string> &start_line)
{
    /* check number of arguments */
    if (start_line.size() != 3)
        return false; // Invalid number of arguments

    /* check the validity of the method from the list */
    for (size_t i = 0; i < methods->size(); i++)
        if (start_line[0] == methods[i])
            _method = methods[i];

    /* In case of no method added */
    if (_method.empty())
        return false; // Invalid method

    /* check _uri: if contains "?" -> fill_in query_string() */
    _uri = start_line[1];
    if (_uri.find("?") != std::string::npos)
    {
        _query_str = _uri.substr(_uri.find("?") + 1, std::string::npos);
        _uri.erase(_uri.find("?"), std::string::npos); // cut unnessesary part
    }

    /* check version of protocol: */
    _version = start_line[2];
    if (_version != HTTP)
        return false; // Invalid version
    _status = Request::HEADERS;
    return true;
}

void Request::parse_init(std::vector<std::string> &split_lines, std::string &orig_lines)
{
    for (size_t i = 0; i < split_lines.size(); i++)
    {
        if (split_lines[i].empty())
        {
            // if orig_lines are all empty -> just delete everything and stopped on the INIT
            split_lines.erase(split_lines.begin()); // delete empty lines
            orig_lines.erase(0, orig_lines.find("\r\n") + 2);
        }
        else
        {
            _status = Request::MTH;
            break ;
        }
    }
}

// <длина блока в HEX><CRLF><содержание блока><CRLF>
bool Request::parse_chunk(std::string &lines)
{
    if (lines.find("\r\n") == std::string::npos)
        return false;
    // TODO: check length in hex



    return true;
}

void Request::print_parsed_request()
{
    std::cout << GREEN"-------PRINT PARSED REQUEST-------" RESET << std::endl;

    std::cout << BLUE"METHOD: " << _method << "\n" RESET;
    std::cout << BROWN"URI: " << _uri << "\n" RESET;
    std::cout << RED"VERSION: "<< _version << "\n" RESET;

    std::map<std::string, std::string>::iterator  it = _headers.begin();
    std::map<std::string, std::string>::iterator  ite = _headers.end();
    std::cout << BLACK"\nHEADERS:\n" RESET;
    for (; it != ite; it++)
        std::cout << "" MAGENTA << (*it).first << RESET": " CYAN << (*it).second << "\n" RESET;
    std::cout << std::endl;
}

void Request::parse_request(std::string &lines)
{
    std::vector<std::string>    split_lines;

    if (_status == Request::DONE)
        return ;
    if (_status == Request::INIT)
    {
        split_lines = split(lines, "\r\n"); // split initial message
        parse_init(split_lines, lines);
        _status = Request::MTH;
    }
    if (_status == Request::MTH)
    {
        std::vector<std::string>    start_line;
        start_line = split(split_lines[0], " ");
        if ((check_start_line(start_line)) == false)
            error_message("Bad request sent by client!"); // TODO: return (?)
    }
    if (_status == Request::HEADERS)
    {
        split_lines = split(lines, "\r\n\r\n"); // remove last lines
        split_lines = split(split_lines[0], "\r\n"); // main split
        set_up_headers(split_lines);
        lines.erase(0, lines.find("\r\n\r\n") + 4); // erase headers
        if (_content_len > 0)
            _status = Request::BODY_PARSE;
        else if (_chunk == true)
            _status = Request::CHUNK;
    }
    if (_status == Request::BODY_PARSE)
    {
        // Lines of characters in the body MUST be limited to 998 characters - https://tools.ietf.org/html/rfc2822.html#section-2.2
        _body = lines;
        _status = Request::DONE;
    }
    if (_status == Request::CHUNK) // https://docs.python.org/3/library/http.client.html
        if ((parse_chunk(lines)) == false)
            error_message("Bad chunk request");

    /* check-print request - delete later */
    print_parsed_request();
}

// TODO: coding special signs from URL: !#%^&()=+ и пробел

std::string Request::find_header(std::string header)
{
    std::map<std::string, std::string>::iterator it = _headers.begin();
    std::map<std::string, std::string>::iterator ite = _headers.end();

    for (; it != ite; it++)
        if ((*it).first == header)
            return (*it).second;
    return NULL;
}

void Request::set_cgi_meta_vars()
{
    std::string header_found;

    if ((header_found = find_header("Authorization")).c_str() != NULL) // check php (script?)
        _env.push_back("AUTH_TYPE=" + header_found);
    _env.push_back("CONTENT_LENGTH=" + std::to_string(_body.size()));
    if ((header_found = find_header("Content-Type")).c_str() != NULL)
        _env.push_back("CONTENT_TYPE=" + header_found);
    _env.push_back("GATEWAY_INTERFACE=CGI/1.1"); // check php (script?)
    
    // _env.push_back("PATH_INFO="); // Location path? which one?
    // _env.push_back("PATH_TRANSLATED=") // ?

    if (!_query_str.empty())
        _env.push_back("QUERY_STRING=" + _query_str);
    else
        _env.push_back("QUERY_STRING="); // the QUERY_STRING MUST be defined as an empty string ("") - RFC3875 (4.1.7)
    
    _env.push_back("REMOTE_ADDR=" + std::to_string(_client->get_s_addr())); // is it right convertion?
    
    // REMOTE_IDENT - check location
    // REMOTE_USER

    if (!_method.empty())
        _env.push_back("REQUEST_METHOD=" + _method); // always?

    // REQUEST_URI - location path?

    _env.push_back("SCRIPT_NAME="); // add script name
    // The leading "/" is not part of the path.  It is optional if the path is NULL 
    
    // SERVER_NAME - get name from server[i]->get_name
    // SERVER_PORT - get port from server[i]->get_port
    
    _env.push_back("SERVER_PROTOCOL=" + _version);

    //set by yourself?
    _env.push_back("SERVER_SOFTWARE=webserv");
}

//дата для даты и для последнего редактирования


std::string Request::server_date() {
	struct tm info;
	struct timeval time;
	char buf[29];
	gettimeofday(&time, NULL);
	std::string s = std::to_string(time.tv_sec);
	strptime(s.c_str(), " %s ", &info);
	strftime (buf, sizeof(buf), "%a, %d %b %Y %X %Z", &info);
	std::string str = buf;
	return "Date: " + str + "\r\n";
}

void Request::createResponse() {
	//std::string date = server_date();
	_response = "";
	if (_method == "HEAD")
	{
		_response += "HTTP/1.1 200 OK"; //OK - status
		_response += "\r\n";
		_headers["Date"] = server_date();
		std::map<std::string, std::string>::iterator beg = _headers.begin();
		std::map<std::string, std::string>::iterator end = _headers.end();
		while (beg != end)
		{
			if ((*beg).second != "")
			{
				_response += (*beg).first;
				_response += ": ";
				_response += (*beg).second;
				_response += "\r\n";
			}
			++beg;
		}
		_response += "Content-Length: ";
		_response += std::to_string(_body.length());
		_response += "\r\n";
	}
	else if (_method == "PUT")
	{

	}
	else if (_method == "GET")
	{

	}
	else if (_method == "POST")
	{

	}
	//else if (/*без метода*/)
	//{
	//
	//}
}

std::string Request::get_response() {
	return _response;
}

std::string Request::createHeader() {
//	_response = "";
//	_response += "Allow: " + "\r\n";
//	_response += "Content-Language: " + "\r\n";
//	_response += "Content-Length: " + "\r\n";
//	_response += "Content-Location: " + "\r\n";
//	_response += "Content-Type: " + "\r\n";
//	_response += "Date: " + "\r\n";
//	_response += "Last-Modified: " + "\r\n";
//	_response += "Location: " + "\r\n";
//	_response += "Retry-After: " + "\r\n";
//	_response += "Server: " + "\r\n";
//	_response += "Transfer-Encoding: " + "\r\n";
//	_response += "WWW-Authenticate: " + "\r\n";
//	return _response;
}


