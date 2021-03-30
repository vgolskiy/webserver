#ifndef WEBSERV_HPP
# define WEBSERV_HPP

/*
** Allowed external functions:
** <iostream>		: std
** <stdlib.h>		: malloc, free, exit
** <unistd.h>		: write, read, rmdir, unlink, fork, getcwd, chdir, lseek, execve, dup, dup2, pipe, usleep
** <fcntl.h>		: open, close, fcntl
** <sys/types.h>	: constant values
** <sys/stat.h>		: mkdir, stat, lstat, fstat
** <sys/wait.h>		: wait
** <signal.h>		: signal, kill
** <dirent.h>		: opendir, readdir, closedir
** <stdio.h>		: strerror
** <sys/errno.h>	: errno
** <sys/time.h>		: gettimeofday
** <time.h>			: strptime, strftime
** <sys/select.h>	: select
** <sys/socket.h>	: socket, accept, listen, send, recv, bind, connect, setsockopt, getsockname
** <arpa/inet.h>	: inet_addr
*/

# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <signal.h>
# include <dirent.h>
# include <stdio.h>
# include <sys/time.h>
# include <time.h>
# include <sys/select.h>
# include <sys/socket.h>
# include <arpa/inet.h>

# include <iostream>
# include <string>
# include <vector>
# include <list>
# include <queue>
# include <stack>
# include <map>
# include <algorithm>

# include "ErrnoException.hpp"
# include "Sock.hpp"
# include "Serv.hpp"
# include "Client.hpp"
# include "Request.hpp"
# include "Response.hpp"
# include "Http.hpp"
# include "error_handler.hpp"
# include "conf_parser.hpp"
# include "conf_utils.hpp"
# include "webserv_utils.hpp"
# include "signal_handler.hpp"

extern bool g_verbose;
extern std::list<Serv*> g_servers;

#endif
