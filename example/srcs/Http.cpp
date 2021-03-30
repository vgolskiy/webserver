#include "Http.hpp"

std::string const Http::methods[] =
{
	"GET",
	"HEAD",
	"POST",
	"PUT",
	"DELETE",
	"CONNECT",
	"OPTIONS",
	"TRACE",
	"PATCH"
};

std::string const Http::protocols[] =
{
	"HTTP/1.1"
};

std::string const Http::headers[] =
{
	"Accept-Charsets",
	"Accept-Language",
	"Allow",
	"Authorization",
	"Content-Language",
	"Content-Length",
	"Content-Location",
	"Content-Type",
	"Date",
	"Host",
	"Last-Modified",
	"Location",
	"Referer",
	"Retry-After",
	"Server",
	"Transfer-Encoding",
	"User-Agent",
	"WWW-Authenticate"
};

static std::string strToLower(std::string const &str)
{
	std::string out = str;
	for (size_t i = 0; i < str.size(); i++)
	{
		out[i] = std::tolower(str[i]);
	}
	return out;
}

bool Http::isValidMethod(std::string const &method)
{
	int sizeofMethods = (int)sizeof(Http::methods) / (int)sizeof(std::string);
	for (int i = 0; i < sizeofMethods; i++)
		if (method == Http::methods[i])
			return true;
	return false;
}

bool Http::isValidProtocol(std::string const &protocol)
{
	int sizeofProtocols = (int)sizeof(Http::protocols) / (int)sizeof(std::string);
	for (int i = 0; i < sizeofProtocols; i++)
		if (protocol == Http::protocols[i])
			return true;
	return false;
}

bool Http::isValidHeader(std::string const &header)
{
	int sizeofHeaders = (int)sizeof(Http::headers) / (int)sizeof(std::string);
	for (int i = 0; i < sizeofHeaders; i++)
		if (strToLower(header) == strToLower(Http::headers[i]))
			return true;
	return false;
}

std::string Http::getRightCaseHeader(std::string const &header)
{
	std::string out = "";
	int sizeofHeaders = (int)sizeof(Http::headers) / (int)sizeof(std::string);
	for (int i = 0; i < sizeofHeaders; i++)
		if (strToLower(header) == strToLower(Http::headers[i]))
		{
			out = Http::headers[i];
			break;
		}
	return out;
}
