#include "Request.hpp"

Request::Request() 
{
    _method = "";
    _query_str = "";
    _version = "";
    _uri = "";
    _body = "";
}

Request::~Request() {}

std::string const	Request::methods[] = {
	"GET",
	"HEAD",
	"POST",
	"PUT",
};

std::string const	Request::headers[] = {
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

std::vector<std::string> Request::get_env()
{
    return _env;
}

void Request::set_up_headers(const std::vector<std::string> &lines)
{
    std::vector<std::string>    tmp;
    bool                        flag;

    for (size_t i = 1; i < lines.size(); i++)
    {
        flag = false;
        tmp = split(lines[i], ":");
        for (size_t j = 0; j != headers->size(); j++)
        {
            if (tmp[0] == headers[j])
            {
                _headers[headers[i]] = tmp[1];
                flag = true;
            }
        }
        if (flag == false)
            error_message("Invalid header!\n"); // Should we do it or just ignore invalid (extra) headers?
        tmp.clear();
    }
}

void Request::check_start_line(const std::vector<std::string> &start_line)
{
    // check number of arguments
    if (start_line.size() != 3)
        error_message("Invalid number of arguments!\n");
    
    // check the validity of the method from the list
    for (size_t i = 0; i < methods->size(); i++)
        if (start_line[0] == methods[i])
            _method = methods[i];
    if (_method.empty())
        error_message("Invalid method!\n");
    
    // check _uri: if contains "?" -> fill_in query_string() ??
    _uri = start_line[1];
    if (_uri.find("?") != std::string::npos)
    {
        _query_str = _uri.substr(_uri.find("?") + 1, std::string::npos);
        _uri.erase(_uri.find("?"), std::string::npos); // cut unnessesary part
    }
    // check absolute path or not
    
    // check version of protocol:
    _version = start_line[2];
    if (_version != HTTP)
        error_message("Invalid version of protocol!\n"); // if error - then what?
}

void Request::parse_request(std::string &lines)
{
    // split initial message (by "\r\n"):
    std::vector<std::string>    split_lines;
    split_lines = split(lines, "\r\n"); // split - from readConfig.cpp

    // split + check start_line
    std::vector<std::string>    start_line;
    start_line = split(split_lines[0], " "); // Split start line with method
    check_start_line(start_line);
    
    // divide headers -> to map;
    set_up_headers(split_lines);
}

std::string Request::find_header(std::string header)
{
    std::map<std::string, std::string>::iterator it = _headers.begin();
    std::map<std::string, std::string>::iterator ite = _headers.end();

    for (; it != ite; it++)
    {
        if ((*it).first == header)
            return (*it).second;
    }
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
