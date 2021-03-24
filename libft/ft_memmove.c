/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mskinner <v.golskiy@ya.ru>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/24 14:27:07 by mskinner          #+#    #+#             */
/*   Updated: 2020/09/11 01:56:19 by mskinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** dest >= src
** bwd copy
** dest < src
** fwd copy
*/

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	unsigned char	*dst_str;
	unsigned char	*src_str;

	if (dst == src || !len)
		return (dst);
	dst_str = (unsigned char *)dst;
	src_str = (unsigned char *)src;
	if (dst_str < src_str)
	{
		while (len--)
			*dst_str++ = *src_str++;
	}
	else
	{
		while (--len)
			*(dst_str + len) = *(src_str + len);
		*(dst_str + len) = *(src_str + len);
	}
	return (dst);
}
