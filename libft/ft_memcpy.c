/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mskinner <v.golskiy@ya.ru>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/24 14:27:05 by mskinner          #+#    #+#             */
/*   Updated: 2020/09/11 01:55:14 by mskinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t len)
{
	char	*dst_str;
	char	*src_str;

	if (dst == src || len == 0)
		return (dst);
	dst_str = (char *)dst;
	src_str = (char *)src;
	while (len--)
		*dst_str++ = *src_str++;
	return (dst);
}
