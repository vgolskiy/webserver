/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   big_changus.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbarbera <fbarbera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 19:15:48 by fbarbera          #+#    #+#             */
/*   Updated: 2021/03/21 15:06:15 by fbarbera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// _buffer - строка 1232\r\nsgfgfgh\r\n2423\r\nfsdfd\r\n0\r\n\r\n
// 

#include "RequestParser.hpp"
#include "parser_config.hpp"





static bool my_find_del(std::string::iterator begin, std::string::iterator end, std::string delimetr, bool &full)
{
	for (size_t n = 0; begin != end; begin++)
	{
		if (*begin != delimetr[n])
			return false;
		n++;
		if (n == delimetr.size())
		{
			full = true;
			return true;
		}
	}
	return true;
}

void RequestParser::_body_chunked() {
	std::string delimetr = "\r\n";
	std::string tmp = _buffer;
	std::string::iterator begin = tmp.begin();
	std::string::iterator end = tmp.end();
	std::string token;
	size_t size_to_delete = 0;
	bool full = false;
	size_t size_buffer = _buffer.size();

	if (size_of_body != 0 && _buffer.length() < size_of_body)
	{
		_status = "read_body_chunked";
		return;
	}

	while (begin != end)
	{
		if (_big_changus)
		{
			if (my_find_del(begin, end, delimetr, full))
			{
				_status = "execute";
				_buffer.clear();
				size_of_body = 0;
				return;
			}
		}
		if (my_find_del(begin, end, delimetr, full) == true && size_of_body == 0)
		{
			if (full)
			{
				if (_count_body == true)
				{
					_body+=token;
					size_to_delete+=token.size() + 2;
					token.clear();
					_count_body = false;
				}
				else
				{
					if (token == "0")
						_big_changus = true;
					size_of_body = this->_to_int(token);
					size_to_delete+=token.size() + 2;
					token.clear();
					_count_body = true;
				}
				full = false;
				begin+=2;
			}
			else
			{
				_status = "read_body_chunked";
				_buffer = _buffer.substr(size_to_delete);
				return;
			}
		}
		else
		{
			if (size_of_body == 0)
			{
				
				token+= *begin;
				begin++;
			}
			else if (size_buffer - size_to_delete > size_of_body)
			{
					token+= tmp.substr(begin - tmp.begin(), size_of_body);
					begin+= size_of_body;
					size_of_body=0;	
			}
			else
			{	
				_status = "read_body_chunked";
				_buffer = _buffer.substr(size_to_delete);
				return ;
			}
		}
	}
	_status = "read_body_chunked";
	_buffer = _buffer.substr(size_to_delete);
	return ;
}
