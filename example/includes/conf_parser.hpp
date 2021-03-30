#ifndef CONF_PARSER_HPP
# define CONF_PARSER_HPP

# include "webserv.hpp"

class Serv;

std::list<Serv*> parseConfFile(std::string const &config);

#endif
