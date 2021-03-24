/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mskinner <v.golskiy@ya.ru>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/24 14:26:58 by mskinner          #+#    #+#             */
/*   Updated: 2020/09/11 01:54:36 by mskinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
**	copies bytes from src to dst
**	if (unsigned char)c occurs in src,
**	copy stops and byte after c in dst is returned.
*/

void	*ft_memccpy(void *dst, const void *src, int c, size_t len)
{
	unsigned char	*dst_str;
	unsigned char	*src_str;

	src_str = (unsigned char *)src;
	dst_str = (unsigned char *)dst;
	while (len--)
	{
		*dst_str = *src_str;
		if (*src_str == (unsigned char)c)
			return (++dst_str);
		dst_str++;
		src_str++;
	}
	return (NULL);
}
