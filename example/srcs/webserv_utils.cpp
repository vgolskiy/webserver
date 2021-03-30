#include "webserv_utils.hpp"

void launchServers(std::list<Serv*> &servers)
{
	std::list<Serv*>::iterator it = servers.begin();
	std::list<Serv*>::iterator ite = servers.end();
	for (; it != ite; it++)
	{
		try
		{
			(*it)->addSocket(AF_INET, SOCK_STREAM, 0);
		}
		catch (std::exception &e)
		{
			std::cerr << "Error: " << ADDSOCKET << " (" << e.what() << ")." << std::endl;
			delete *it;
			servers.erase(it);
		}
	}
}

void setFdSets(std::list<Serv*> const &servers, fd_set &readFdSet, fd_set &writeFdSet, int &fdMax)
{
	std::list<Serv*>::const_iterator it = servers.begin();
	std::list<Serv*>::const_iterator ite = servers.end();
	for (; it != ite; it++)
	{
		(*it)->addReadFdToSet(readFdSet, fdMax);
		(*it)->addWriteFdToSet(writeFdSet, fdMax);
	}
}

void addNewClients(std::list<Serv*> &servers, fd_set const &readFdSet)
{
	std::list<Serv*>::const_iterator it = servers.begin();
	std::list<Serv*>::const_iterator ite = servers.end();
	for (; it != ite; it++)
	{
		if (FD_ISSET((*it)->getSocket()->getSockfd(), &readFdSet))
		{
			(*it)->addClient();
		}
	}
}

void interactWithClients(std::list<Serv*> &servers, fd_set const &readFdSet, fd_set const &writeFdSet)
{
	std::list<Serv*>::const_iterator it = servers.begin();
	std::list<Serv*>::const_iterator ite = servers.end();
	for (; it != ite; it++)
	{
		(*it)->interactWithClients(readFdSet, writeFdSet);
	}
}

void deleteTimeoutServersAndClients(std::list<Serv*> &servers, long timeoutServer, long timeoutClient)
{
	std::list<Serv*>::const_iterator it = servers.begin();
	std::list<Serv*>::const_iterator ite = servers.end();
	for (; it != ite; it++)
	{
		if ((*it)->getStatus() != Serv::DEAD &&
			get_ms() - (*it)->getWaitingTime() > timeoutServer * 1000)
		{
			std::cout << "Server waiting time out: ";
			delete *it;
			it = servers.erase(it);
			continue;
		}
		(*it)->deleteTimeoutClients(timeoutClient * 1000);
	}
	if (servers.empty() == true)
		deleteServersShowMessageAndExit(servers, TIMEOUT);
}

long get_ms()
{
	struct timeval cur_time;

	gettimeofday(&cur_time, NULL);
	long ms = cur_time.tv_sec * 1000 + cur_time.tv_usec / 1000;
	return ms;
}

void deleteServers(std::list<Serv*> &servers)
{
	if (servers.empty() == false)
	{
		std::list<Serv*>::iterator it = servers.begin();
		std::list<Serv*>::iterator ite = servers.end();
		for (; it != ite; ++it)
		{
			delete *it;
		}
		servers.clear();
	}
}

std::string fileToStr(char const filename[])
{
	int fd;

	if ((fd = open(filename, O_RDONLY)) == -1)
		throw ErrnoException();
	std::string buf;
	char bufc[BUFFERSIZE];
	int ret;
	while ((ret = read(fd, &bufc, BUFFERSIZE)))
	{
		if (ret == -1)
			throw ErrnoException();
		buf += std::string(bufc, ret);
	}
	close(fd);
	return buf;
}

void strToFd(std::string const &str, int fd)
{
	std::streambuf *backup = std::cerr.rdbuf();
	dup2(fd, 2);
	std::cerr << str;
	std::cerr.rdbuf(backup);
}

std::string base64encode(std::string const &decoded)
{
	std::string encoded;
	char const base[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	int bits_decoded = decoded.size() * 8;
	int bits_encoded = (bits_decoded / 6 + ((bits_decoded % 6) != 0)) * 6;
	bool bits[bits_encoded];
	for (int byte = 0; byte < (int)decoded.size(); ++byte)
	{
		for (int bit = 7; bit >= 0; --bit)
			bits[byte * 8 + bit] = (decoded[byte] >> (7 - bit)) & 1;
	}
	for (int bit = bits_decoded; bit < bits_encoded; ++bit)
		bits[bit] = 0;
	int ind;
	for (int byte = 0; byte < bits_encoded / 6; byte++)
	{
		ind = 0;
		for (int bit = 0; bit < 6; bit++)
			ind += bits[byte * 6 + bit] << (5 - bit);
		encoded += base[ind];
	}
	if (decoded.size() % 3 == 1)
		encoded += "==";
	else if (decoded.size() % 3 == 2)
		encoded += "=";
	return encoded;
}

std::string base64decode(std::string encoded)
{
	std::string decoded;
	std::string const base = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	for (int i = 0; i < 2; i++)
		if (encoded[encoded.size() - 1] == '=')
			encoded.erase(encoded.size() - 1);
	int bits_encoded = encoded.size() * 6;
	int bits_decoded = bits_encoded / 8 * 8;
	bool bits[bits_decoded];
	for (int byte = 0; byte < (int)encoded.size(); ++byte)
	{
		for (int bit = 5; bit >= 0; --bit)
			bits[byte * 6 + bit] = (base.find(encoded[byte]) >> (5 - bit)) & 1;
	}
	char sym;
	for (int byte = 0; byte < bits_decoded / 8; byte++)
	{
		sym = 0;
		for (int bit = 0; bit < 8; bit++)
			sym += bits[byte * 8 + bit] << (7 - bit);
		decoded += sym;
	}
	return decoded;
}

std::string ntoaddr(int n)
{
	std::string addr;
	struct in_addr in;
	in.s_addr = (in_addr_t)n;
	unsigned char *bytes = (unsigned char *) &in;
	for (int i = 0; i < 4; i++)
	{
		addr += std::to_string(static_cast<int>(bytes[i]));
		if (i < 3)
			addr += '.';
	}
	return addr;
}
