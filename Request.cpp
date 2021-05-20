/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mskinner <v.golskiy@ya.ru>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/15 19:29:16 by mskinner          #+#    #+#             */
/*   Updated: 2021/05/20 16:28:20 by mskinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request(Client *client, const int i) : _i(i) {
    _method = "";
    _param = "";
    _version = "";
    _uri = "";
    _body = "";
	_location = "";
    _client = client;
    _remain_len = 0;
    _status = Request::REQUEST_METHOD;
    _content_len = 0;
    _chunk = false;
	_script_path = NULL;
	_script_name = NULL;
	_requested_index = "";
	_status_code = 0;
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
	"Allow", //
	"Authorization",
	"Content-Language",
	"Content-Length",
	"Content-Location",
	"Content-Type",
	"Date",
	"Host",
	"Last-Modified",
	"Location", //
	"Referer",
	"Retry-After", //
	"Server", //
	"Transfer-Encoding",
	"User-Agent",
	"WWW-Authenticate", //
};

int Request::get_remain_len() const {
	return (_remain_len);
}

int Request::get_status() const {
	return (_status);
}

std::string Request::get_uri() const {
	return (_uri);
}

std::string	Request::get_body(void) const {
	return (_body);
}

std::map<std::string, std::string> Request::get_headers(void) const {
	return (_headers);
}

std::string Request::get_method() const {
	return (_method);
}

const char*	Request::get_script_name(void) const {
	return (_script_name);
}

std::string Request::get_location_name(void) const {
	return (_location);
}

std::string	Request::get_requested_index(void) const {
	return (_requested_index);
}

std::string	Request::get_uri_parameters(void) const {
	return (_param);
}

std::string	Request::get_authorization(void) const {
	return (_authorize);
}

void		Request::set_request_status(int status) {
	_status = status;
}

void		Request::set_status_code(int code){
	_status_code = code;
}

int			Request::get_status_code(void) const {
	return (_status_code);
}

void remove_spaces(std::string &str) 
{
    char* chr = const_cast<char*>(str.c_str());
    int i = 0;

    while (chr[i] == ' ')
        i++;
    str = std::string(&chr[i]);
}

bool Request::set_up_headers(std::string &lines) {
	t_location*	loc = get_location(g_servers[_i], _location);
    std::vector<std::string>	tmp;
	std::size_t					pos = lines.find(CRLF) == std::string::npos ? 0 : lines.find(CRLF);

	//Cycle is not ending until closing "/r/n" for HEADER (in case of one time recv reading)
	//Cycle starts in case of available header for parsing "<header>/r/n"
    while (pos) {
        tmp = split(lines.substr(0, lines.find(CRLF)), ":");
        if(tmp.size() < 2) {
            error_message("Invalid number of arguments for the header: " + tmp[0]);
            return (false);
        }
        for (size_t i = 0; i < tmp.size(); ++i)
            remove_spaces(tmp[i]);
        for (size_t j = 0; j != headers->size(); ++j) {
            if (tmp[0] == headers[j]) {
                if (find_header(tmp[0])) {
                    error_message("Repetitive header in the request");
                    return (false);
                }
                if (tmp[0] == AUTHORIZATION) {
					if (!loc->auth.size()) {
					    error_message("Autorization error: excessive header for this location");
                        return (false);
					}
                    tmp = split(tmp[1], " ");
                    if (tmp.size() != 2) {
                        error_message("Autorization error: invalid number of arguments");
                        return (false);
                    }
					else if (tmp[0] != "Basic") {
                        error_message("Autorization error: unsupported authorization type");
                        return (false);
					}
					//encrypted pair <user>:<password>
					_authorize = tmp[1];
                }
                else if ((tmp[0] == HOST) && (tmp.size() == 3)) {
                    std::string to_ret;
                    to_ret.append(tmp[1]);
                    to_ret += ":";
                    to_ret.append(tmp[2]);
                    _headers[headers[j]] = to_ret;
                }
                else
                    _headers[headers[j]] = tmp[1];
            }
        }
        if (tmp[0] == CONTENT_LEN) {
            try {
                _content_len = std::stoi(tmp[1]);
                _remain_len = _content_len;
            }
            catch(const std::exception& e) {
                error_message(e.what());
				_status = Request::BAD_REQ;
				_status_code = 400;
				return (false);
            }
			if (_content_len < 0) {
				_status = Request::BAD_REQ;
				_status_code = 400;
				return (false);			
			}
        }
        if ((tmp[0] == TRANSF_ENCODE) && (tmp[1] == CHUNKED))
            _chunk = true;
        tmp.clear();
		lines.erase(0, lines.find(CRLF) + 2);
		pos = lines.find(CRLF) == std::string::npos ? 0 : lines.find(CRLF);
    }
    return (true);
}

bool Request::check_start_line(const std::vector<std::string> &start_line) {
	t_location	*loc;
	std::string	tmp;

    /* check number of arguments */
    if (start_line.size() != 3)
        return (false);
    /* check version of protocol: */
    _version = start_line[2];
    if (_version != HTTP)
        return (false);
    /* check _uri: if contains "?" -> fill_in query_string() */
    _uri = start_line[1];
    if (_uri.find("?") != std::string::npos) {
        _param = _uri.substr(_uri.find("?") + 1, std::string::npos);
        _uri.erase(_uri.find("?"), std::string::npos);
    }
	/*
	** Verification that location is among available in server config
	** Method is available in found location
	*/
	_location = _uri.rfind("/") ? _uri.substr(0, _uri.rfind("/")) : _uri;
	if ((loc = get_location(g_servers[_i], _location))) {
    	/* check the validity of the method from the location methods list */
    	for (std::size_t i = 0; i < loc->methods.size(); ++i) {
        	if (start_line[0] == loc->methods[i]) {
            	_method = loc->methods[i];
				break ;
			}
		}
		if (_uri.find(".") != std::string::npos) {
			//cut the file name without CLRF
			tmp = _uri.substr(_uri.rfind("/") + 1, _uri.length() - 4);
			for (std::size_t i = 0; i < loc->index.size(); ++i) {
				if (loc->index[i] == tmp)
					_requested_index = tmp;
			}
		}
	}
	//If there is no needed location / method or uri contains file that not in index 
	if ((!loc) || (!_method.length()) 
		|| (tmp.length() && (!_requested_index.length())))
		return (false);
	_status = Request::HEADERS;
    return (true);
}

// <длина блока в HEX><\r\n><содержание блока><\r\n>
bool Request::parse_chunk_size(std::string &lines) {
    std::size_t	pos;
    std::string	tmp;

	//If there is no content / length is not hexadecimal value
    if (((pos = lines.find(CRLF)) == std::string::npos)
		|| (!is_hex(tmp = lines.substr(0, pos))))
        return (false);
    _remain_len = std::strtol(tmp.c_str(), 0, 16);
	//in case of overflow
	if ((errno == ERANGE) || (_remain_len < 0)) {
		_status = Request::BAD_REQ;
		_status_code = 400;
		return (false);
	}
    lines.erase(0, lines.find(CRLF) + 2);
    _status = Request::CHUNK_DATA;
    return (true);
}

void Request::parse_chunk_data(std::string &lines) {
	std::size_t	pos;
	std::string	tmp;
	t_location*	loc = get_location(g_servers[_i], _location);

    if ((!_remain_len) && (lines == CRLF)) {
		_content_len = _body.length();
        _status = Request::DONE;
        return ;
    }
	//string was not fully readed from the first time
	if ((pos = lines.find(CRLF)) == std::string::npos)
		return ;
	tmp = lines.substr(0, pos);
    if (_remain_len < (int)tmp.length())
		tmp = lines.substr(0, _remain_len);
	lines.erase(0, pos + 2);
	//write to body string by string
	if (_remain_len) {
		if (_body.length()) {
			_body += "\n";
			_body += tmp;
		}
		else
        	_body += tmp;
		if ((loc->max_body > 0) && ((int)_body.length() > loc->max_body)) {
			_status = Request::BAD_REQ;
			_status_code = 400;
			return ;
		}
		_remain_len -= tmp.length();
	}
    _status = Request::CHUNK;
    return ;
}

void Request::print_parsed_request()
{
    std::cout << GREEN"-------PRINT PARSED REQUEST-------" RESET << std::endl;

    std::cout << BLUE"METHOD: " << _method << "\n" RESET;
    std::cout << BROWN"URI: " << _uri << "\n" RESET;
    std::cout << RED"VERSION: "<< _version << "\n" RESET;

	if (_status != Request::BAD_REQ) {
    	std::map<std::string, std::string>::iterator  it = _headers.begin();
    	std::map<std::string, std::string>::iterator  ite = _headers.end();
    	std::cout << BLACK"\nHEADERS:\n" RESET;
    	for (; it != ite; it++)
        	std::cout << "" MAGENTA << (*it).first << RESET": " CYAN << (*it).second << "\n" RESET;

    	std::cout << "" MAGENTA << "Authorization" << RESET": " CYAN << _authorize << "\n" RESET;
    	std::cout << std::endl;
	}
}

//Verifying that remain length is zero, changing status accordingly
void Request::verify_body() {
	if (!_remain_len)
		_status = Request::DONE;
	else {
		//in case of less body data only
		error_message("Bad request sent by client: wrong content length");
		_status = Request::BAD_REQ;
	}
}

void Request::parse_request(std::string &lines) {
	std::string	tmp;
	std::size_t pos;
	t_location*	loc = get_location(g_servers[_i], _location);
    
    if (_status == Request::DONE || _status == Request::BAD_REQ)
        return ;
	if ((pos = lines.find(CRLF)) == std::string::npos)
		return ;
    if (_status == Request::REQUEST_METHOD || _status == Request::HEADERS) {
        if ((_status == Request::REQUEST_METHOD) && pos) {
            std::vector<std::string>    start_line;

            start_line = split(lines.substr(0, pos), " ");
            if (!check_start_line(start_line)) {
                error_message("Bad request sent by client: wrong request first line");
                _status = Request::BAD_REQ;
				return ;
            }
			lines.erase(0, pos + 2);
			return ;
        }
        if ((_status == Request::HEADERS) && pos) {
            if (!(set_up_headers(lines)))
                _status = Request::BAD_REQ;
			return ;
        }
		if (!pos) {
			if (_status != Request::HEADERS) {
				error_message("Bad request sent by client: empty line");
				_status = Request::BAD_REQ;
				return ;
			}
			lines.erase(0, 2); //second CRLF remove
            if ((_content_len > 0) || (_method == "POST")
				|| (_method == "PUT"))
                _status = Request::BODY_PARSE;
            else if (_chunk == true)
                _status = Request::CHUNK;
            else
                _status = Request::DONE;
			return ;
		}
    }
	//adding tmp strings to body until remain length is above zero
	//in case of max_body > content_length or content_length < body length -> bad request
    if ((_status == Request::BODY_PARSE) && pos) {
		tmp = lines.substr(0, pos);
		if ((_method != "POST") && (_method != "PUT")
			&& (_remain_len < (int)tmp.length())) {
			_status = Request::BAD_REQ;
			_status_code = 400;
			return ;
		}
		lines.erase(0, pos + 2);
		if (_remain_len) {
        	_body += tmp;
			_remain_len -= tmp.length();
		}
		else if ((_method == "POST") || (_method == "PUT"))
			_body += tmp;
		if ((_content_len > 0) && (((int)_body.length() > _content_len)
			|| ((loc->max_body > 0) && (_content_len > loc->max_body)))) {
			_status = Request::BAD_REQ;
			_status_code = 400;
			return ;	
		}
		return ;
    }
	//Quit in case double CLRF
	if ((_status == Request::BODY_PARSE) && !pos) {
		verify_body();
		return ;
	}
    if ((_status == Request::CHUNK) && pos) {
        if (!(parse_chunk_size(lines))) {
			error_message("Bad request sent by client: wrong chunk size");
			_status = Request::BAD_REQ;
			return ;
		}
	}
	//Quit in case double CLRF
	else if ((_status == Request::CHUNK) && !pos) {
		verify_body();
		return ;
	}
    if (_status == Request::CHUNK_DATA)
        parse_chunk_data(lines);
	return ;
}

// TODO: coding special signs from URL: !#%^&()=+ и пробел
std::string* Request::find_header(std::string header) {
    std::string* empty_head = NULL;
    std::map<std::string, std::string>::iterator it = _headers.begin();
    std::map<std::string, std::string>::iterator ite = _headers.end();

    for (; it != ite; it++)
        if ((*it).first == header)
            return (&(*it).second);
    return (empty_head);
}

void Request::set_cgi_meta_vars() {
	t_location*	loc = get_location(g_servers[_i], _location);
	bool		php = ((tail(_uri, 4) == ".php") && (loc->php_path.length())) ? true : false;
    std::string*	header_found;

	/*
	** The "basic" authentication scheme is based on the model that the
	** client must authenticate itself with a user-ID and a password
	*/
    if (!php && (!_authorize.length()))
        _env["AUTH_TYPE"] = "Basic";
    _env["CONTENT_LENGTH"] = std::to_string(_body.size());
    if ((header_found = find_header("Content-Type")))
        _env["CONTENT_TYPE"] = (*header_found);
	if (!php)
    	_env["GATEWAY_INTERFACE"] = "CGI/1.1";

	//Defines location (uri)
	//Subject: Because you wont call the cgi directly use the full path as PATH_INFO
    _env["PATH_INFO"] = _uri;

	//Full path to content: folder with script + script file name
    _env["PATH_TRANSLATED"] = _script_path;

	// the QUERY_STRING MUST be defined as an empty string ("") - RFC3875 (4.1.7)
    if (!_param.empty())
        _env["QUERY_STRING"] = _param;
    else
        _env["QUERY_STRING"];
    
	//Internet host address convertion from binary form into the IPv4 numbers-and-dots notation
    _env["REMOTE_ADDR"] + inet_ntoaddr(_client->get_s_addr());
    
    // REMOTE_IDENT - location authentification
    // REMOTE_USER
	if (!_authorize.length()) {
		_env["REMOTE_IDENT"] = _authorize.substr(0, _authorize.find(":"));
		_env["REMOTE_USER"] = _authorize.substr(0, _authorize.find(":"));
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
		_env["SERVER_NAME"] = g_servers[_i]->name;
    	//Just name of our program
    	_env["SERVER_SOFTWARE"] = g_servers[_i]->name;
	}
    // SERVER_PORT - get port from server[i]->get_port
    _env["SERVER_PORT"] = std::to_string(ntohs(g_servers[_i]->port.front()));
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
void Request::parse_script_file_name() {
	t_location*	loc = get_location(g_servers[_i], _location);
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
	// TODO: change all exit_errors to throw error -> write appropriate response

	// TODO: the cgi should be run in the correct directory for relativ path file access (subject)
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

void Request::read_cgi()
{
	std::string			res = "";
	std::string 		status_line = "Status: ";
	std::string			header = "";
	std::stringstream 	ss;

    std::ifstream		inf(TMP);
	if (!inf)
		throw std::runtime_error(TMP); // add try in upper level
	ss << inf.rdbuf();
	res += ss.str();
	res += "\n";
	unlink(TMP); // the same as rm
	header = res.substr(0, res.find(CRLF_2X));
	// CGI programs can send status information as part of a virtual document (example below)
	if (header.find(status_line) != std::string::npos)
	{
		header.erase(0, header.find(status_line) + status_line.length());
		_status_code = ft_atoi(header.c_str());
	}
	res.erase(0, res.find(CRLF_2X) + 4);
	_body = res;
}

/* 
Example of cgi virtual document.
The Status header consists of a three-digit numerical status code,
followed by a string representing the code.
#!/usr/local/bin/perl
$remote_host = $ENV{'REMOTE_HOST'};
print "Content-type: text/plain", "\n";
if ($remote_host eq "bu.edu") {
        print "Status: 200 OK", "\n\n";
        print "Great! You are from Boston University!", "\n";
} else {
        print "Status: 400 Bad Request", "\n\n";
        print "Sorry! You need to access this from Boston University!", "\n";
}
exit (0);
*/
