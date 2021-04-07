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
            error_message("Invalid header!\n"); // Should we do it or just ignore?
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
        _query_str = _uri.substr(_uri.find("?") + 1, std::string::npos);
    // check absolute path or not
    
    // check version of protocol:
    _version = start_line[2];
    if (_version != HTTP)
        error_message("Invalid version of protocol!\n");
}

void Request::parse_request(std::string &lines)
{
    // split initial message (by "\r\n"):
    std::vector<std::string>    split_lines;
    split_lines = split(lines, "\r\n"); // from readConfig.cpp

    // split + check start_line
    std::vector<std::string>    start_line;
    start_line = split(split_lines[0], " "); // Split start line with method
    check_start_line(start_line);
    
    // divide headers -> to map;
    set_up_headers(split_lines);
}
