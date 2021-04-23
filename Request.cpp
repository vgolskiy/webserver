/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mskinner <v.golskiy@ya.ru>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/15 19:29:16 by mskinner          #+#    #+#             */
/*   Updated: 2021/04/23 11:58:41 by mskinner         ###   ########.fr       */
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
	_location = "";
    _client = client;
    _remain_len = 0;
    _status = INIT;
    _content_len = -1;
    _chunk = false;
	_script_path = NULL;
	_script_name = NULL;
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

int Request::get_remain_len() {
	return _remain_len;
}

int Request::get_status() {
	return _status;
}

void Request::cut_remain_len(int to_cut) {
	_remain_len -= to_cut;
}

void remove_spaces(std::string &str) 
{
    char* chr = const_cast<char*>(str.c_str());
    int i = 0;

    while (chr[i] == ' ')
        i++;
    str = std::string(&chr[i]);
}

bool Request::set_up_headers(const std::vector<std::string> &lines)
{
    std::vector<std::string>    tmp;
    bool                        flag;

    for (size_t i = 1; i < lines.size(); i++)
    {
        flag = false;
        tmp = split(lines[i], ":"); // TODO: special case for localhost:XXXX
        for (size_t i = 0; i < tmp.size(); i++)
            remove_spaces(tmp[i]);
        for (size_t j = 0; j != headers->size(); j++)
        {
            if (tmp[0] == headers[j])
            {
                if (find_header(tmp[0]) != "NULL")
                {
                    error_message("Repetitive header in the request!");
                    return false ;
                }
                _headers[headers[j]] = tmp[1];
                flag = true;
            }
        }
        if (tmp[0] == "Content-Length")
        {   
            try
            {
                _content_len = std::stoi(tmp[1]);
                _remain_len = _content_len;
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
    return true;
}

bool Request::check_start_line(const std::vector<std::string> &start_line, const int i) {
	t_location	*loc;

    /* check number of arguments */
    if (start_line.size() != 3)
        return false;
    /* check version of protocol: */
    _version = start_line[2];
    if (_version != HTTP)
        return false;
    /* check _uri: if contains "?" -> fill_in query_string() */
    _uri = start_line[1];
    if (_uri.find("?") != std::string::npos)
    {
        _query_str = _uri.substr(_uri.find("?") + 1, std::string::npos);
        _uri.erase(_uri.find("?"), std::string::npos);
    }
	/*
	** Verification that location is among available in server config
	** Method is available in found location
	*/
	_location = _uri.rfind("/") ? _uri.substr(0, _uri.rfind("/")) : _uri;
	if ((loc = get_location(g_servers[i], _location))) {
    	/* check the validity of the method from the location methods list */
    	for (size_t i = 0; i < loc->methods.size(); i++) {
        	if (start_line[0] == loc->methods[i])
            	_method = loc->methods[i];
		}
	}
	if (!loc || (!_method.length()))
		return false;
    _status = Request::HEADERS;
    return true;
}

void Request::parse_init(std::vector<std::string> &split_lines, std::string &orig_lines)
{
    for (size_t i = 0; i < split_lines.size(); i++)
    {
        if (split_lines[i].empty())
        {
            split_lines.erase(split_lines.begin());
            orig_lines.erase(0, orig_lines.find("\r\n") + 2);
        }
        else
        {
            _status = Request::MTH;
            break ;
        }
    }
}


// <длина блока в HEX><\r\n><содержание блока><\r\n>
bool Request::parse_chunk_size(std::string &lines)
{
    size_t      pos;
    std::string str;

    if ((pos = lines.find("\r\n")) == std::string::npos) // If there is nothing
        return false;
    str = lines.substr(0, pos);
    _content_len = std::strtol(lines.c_str(), 0, 16); // TODO: change base
    _remain_len = _content_len;
    lines.erase(0, lines.find("\r\n") + 2);
    _status = Request::CHUNK_DATA;

    return true;
}

bool Request::parse_chunk_data(std::string &lines)
{
    if (_content_len == 0 && lines == "\r\n")
    {
        _status = Request::DONE;
        return false;
    }
    if (_content_len + 2 <= (int)lines.size()) // +2 - erased "\r\n"
    {
        _body += lines.substr(0, _content_len);
        lines.erase(0, _content_len); // +2 - to erase "\r\n"
        if (lines.find("\r\n") != 0) // so there is no \r\n
        {
            std::cout << "Bad request chunk data!\n";
            _status = Request::BAD_REQ;
            return false;
        }
        lines.erase(0, 2);
        _status = Request::CHUNK;
        return true;
    }
    std::cout << "Bad request chunk length!\n";
    return false;
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

void Request::parse_request(std::string &lines, const int i)
{
    std::vector<std::string>    split_lines;
    
    if (_status == Request::DONE || _status == Request::BAD_REQ)
        return ;
    if (_status == Request::INIT || _status == Request::MTH || _status == Request::HEADERS)
    {
        split_lines = split(lines, "\r\n");
        if (_status == Request::INIT && !split_lines.empty())
            parse_init(split_lines, lines);
        if (_status == Request::MTH && !split_lines.empty())
        {
            std::vector<std::string>    start_line;
            start_line = split(split_lines[0], " ");
            if (!check_start_line(start_line, i))
            {
                error_message("Bad request sent by client!");
                _status = Request::BAD_REQ;
            }
        }
        if (_status == Request::HEADERS && !split_lines.empty())
        {
            if (lines.find("\r\n\r\n", 0) != std::string::npos)
            {
                split_lines = split(lines, "\r\n\r\n");
                split_lines = split(split_lines[0], "\r\n");
            }
            if ((set_up_headers(split_lines)) == false)
            {
                std::cout << "Bad request headers\n";
                _status = BAD_REQ;
            }
            if (lines.find("\r\n\r\n", 0) != std::string::npos && _status != BAD_REQ)
            {
                lines.erase(0, lines.find("\r\n\r\n") + 4);
                if (_content_len > 0)
                {
                    _status = Request::BODY_PARSE;
                    _remain_len -= lines.size();
                }
                else if (_chunk == true)
                    _status = Request::CHUNK;
                else
                    _status = Request::DONE;
            }
        }
    }
    if (_status == Request::BODY_PARSE)
    {
        _body = (_content_len <= (int)lines.size()) ? lines : "";
        _status = _body.size() > 0 ? Request::DONE : Request::BODY_PARSE;
    }
    if (_status == Request::CHUNK)
        if (!(parse_chunk_size(lines)))
            return ;
    if (_status == Request::CHUNK_DATA)
        if (parse_chunk_data(lines))
            return(parse_request(lines, i));
    /* check-print request - delete later */
    if (_status == Request::BAD_REQ)
    {
        std::cout << "BAD REQUEST CLIENT: " << std::endl;
        print_parsed_request();
    }
    if (_status == Request::DONE)
    {
        print_parsed_request();
        std::cout << "Body: " << _body << std::endl;
    }
}

// TODO: coding special signs from URL: !#%^&()=+ и пробел

std::string Request::find_header(std::string header)
{
    std::string empty_head = "NULL"; 
    std::map<std::string, std::string>::iterator it = _headers.begin();
    std::map<std::string, std::string>::iterator ite = _headers.end();

    for (; it != ite; it++)
        if ((*it).first == header)
            return (*it).second;
    return empty_head;
}

void Request::set_cgi_meta_vars(const int i) {
	t_location*	loc = get_location(g_servers[i], _location);
	bool		php = ((tail(_uri, 4) == ".php") && (loc->php_path.length())) ? true : false;
    std::string	header_found;

	/*
	** The "basic" authentication scheme is based on the model that the
	** client must authenticate itself with a user-ID and a password
	*/
    if (!php && ((header_found = find_header("Authorization")) != "NULL"))
        _env["AUTH_TYPE"] = header_found;
    _env["CONTENT_LENGTH"] = std::to_string(_body.size());
    if ((header_found = find_header("Content-Type")) != "NULL")
        _env["CONTENT_TYPE"] = header_found;
	if (!php)
    	_env["GATEWAY_INTERFACE"] = "CGI/1.1";

	//Defines location (uri)
	//Subject: Because you wont call the cgi directly use the full path as PATH_INFO
    _env["PATH_INFO"] = _uri;

	//Full path to content: folder with script + script file name
    _env["PATH_TRANSLATED"] = _script_path;

	// the QUERY_STRING MUST be defined as an empty string ("") - RFC3875 (4.1.7)
    if (!_query_str.empty())
        _env["QUERY_STRING"] = _query_str;
    else
        _env["QUERY_STRING"];
    
	//Internet host address convertion from binary form into the IPv4 numbers-and-dots notation
    _env["REMOTE_ADDR"] + inet_ntoaddr(_client->get_s_addr());
    
    // REMOTE_IDENT - location authentification
    // REMOTE_USER
	if (!loc->auth.begin()->first.empty()) {
		_env["REMOTE_IDENT"] = loc->auth.begin()->first;
		_env["REMOTE_USER"] = loc->auth.begin()->first;
	}

	//Method was verified against possible methods upon location
	if (!php)
		_env["REQUEST_METHOD"] = _method;

    // REQUEST_URI - location path (uri in Request) 
	_env["REQUEST_URI"] = _uri;

	//Full path name of the file to execute
	// The leading "/" is not part of the path.  It is optional if the path is NULL
	if (!php) {
		_env["SCRIPT_NAME"] = _script_name ? _script_name : _uri;
		// SERVER_NAME - get name from server[i]->get_name
		_env["SERVER_NAME"] = g_servers[i]->name;
    	//Just name of our program
    	_env["SERVER_SOFTWARE"] = g_servers[i]->name;
	}
    // SERVER_PORT - get port from server[i]->get_port
    _env["SERVER_PORT"] = std::to_string(ntohs(g_servers[i]->port.front()));
    _env["SERVER_PROTOCOL"] = _version;
}

//Changing way of data storage map<string,string> -> vector<const char*>
std::vector<const char*>	Request::convert_cgi_meta_vars() {
	std::map<std::string, std::string>::const_iterator	it;
	std::vector<const char*>	env;

	for (it = _env.begin(); it != _env.end(); ++it)
		env.push_back(((*it).first + "=" + (*it).second).c_str());
	return (env);
}

// _uri = /<location>/<arg>
// Subject: Your program should call the cgi with the file requested as first argument
void Request::parse_script_file_name(const int i) {
	t_location*	loc = get_location(g_servers[i], _location);
	size_t		pos = _uri.rfind("/");

	if ((tail(_uri, 4) == ".php") && (loc->php_path.length())) {
		_script_path = loc->php_path.c_str();
		_script_name = (_uri.substr(pos == std::string::npos ? 0 : pos + 1, std::string::npos)).c_str();
	}
	else
		_script_path = loc->cgi_path.c_str();
}

// Functions to use: execve, dup2, pipe, fork, waitpid
void Request::run_cgi_request() {
	/*
	** envp is an array of pointers to strings, conventionally of the
    ** form key=value, which are passed as the environment of the new
    ** program.  The envp array must be terminated by a NULL pointer.
	*/
	std::vector<const char*>	envp = convert_cgi_meta_vars();
	/*
	** argv is an array of pointers to strings passed to the new program
    ** as its command-line arguments.  By convention, the first of these
    ** strings (i.e., argv[0]) should contain the filename associated
    ** with the file being executed.  The argv array must be terminated
    ** by a NULL pointer.
	** _script_path contains directory to php/cgi binary
	** _script_name is NULL for cgi / gets php file name from _uri
	*/
	const char*	args[] = {_script_path, _script_name, NULL};
    int 		pipe_fds[2];
    pid_t 		pid;
    int 		tmp_fd;

	//In case of any problems during fork: exit with errno code
    //Opening file to write in
    if (((tmp_fd = open(TMP, O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0)
		|| ((pipe(pipe_fds)) < 0))
    	exit_error(errno);
    pid = fork();
    if (pid < 0)
        exit_error(errno);
    else if (!pid) {
		close(pipe_fds[PIPE_IN]);
		// stdin подключается к выходу канала
        if (dup2(pipe_fds[PIPE_OUT], STDIN_FILENO) < 0)
			exit_error(errno);
        close(pipe_fds[PIPE_OUT]);
		// stdout подключается к временному файлу - происходит запись во временный файл
		if ((dup2(tmp_fd, STDOUT_FILENO) < 0)
			|| (execve(_script_path, (char *const *)args, (char *const *)&envp[0]) < 0))
			exit_error(errno);
    }
    else {
		int	status = 0;
        close(pipe_fds[PIPE_OUT]);
        waitpid(pid, &status, 0);
        close(pipe_fds[PIPE_IN]);
        close(tmp_fd);
    }
}

//дата для даты и для последнего редактирования
//struct stat {
//	dev_t         st_dev;      /* устройство */
//	ino_t         st_ino;      /* inode */
//	mode_t        st_mode;     /* режим доступа */
//	nlink_t       st_nlink;    /* количество жестких ссылок */
//	uid_t         st_uid;      /* идентификатор пользователя-владельца */
//	gid_t         st_gid;      /* идентификатор группы-владельца */
//	dev_t         st_rdev;     /* тип устройства */
//	/* (если это устройство) */
//	off_t         st_size;     /* общий размер в байтах */
//	blksize_t     st_blksize;  /* размер блока ввода-вывода */
//	/* в файловой системе */
//	blkcnt_t      st_blocks;   /* количество выделенных блоков */
//	time_t        st_atime;    /* время последнего доступа */
//	time_t        st_mtime;    /* время последней модификации */
//	time_t        st_ctime;    /* время последнего изменения */
//};


std::string Request::server_date() {
	struct tm info;
	struct timeval time;
	char buf[29];
	gettimeofday(&time, NULL);
	std::string s = std::to_string(time.tv_sec);

	strptime(s.c_str(), " %s ", &info);
	//форматировать время как строку
	//a - день недели, d - день месяца и тд
	strftime(buf, sizeof(buf), "%a, %d %b %Y %X %Z", &info);
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
