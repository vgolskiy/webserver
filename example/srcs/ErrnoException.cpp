#include "ErrnoException.hpp"

char const *ErrnoException::what() const throw()
{
	return strerror(errno);
}
