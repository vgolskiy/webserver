#ifndef ERRNOEXCEPTION_HPP
# define ERRNOEXCEPTION_HPP

# include <iostream>
# include <sys/errno.h>

class ErrnoException: public std::exception
{
public:
	virtual char const *what() const throw();
};

#endif
