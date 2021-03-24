/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mskinner <v.golskiy@ya.ru>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/24 14:27:10 by mskinner          #+#    #+#             */
/*   Updated: 2020/09/11 01:56:19 by mskinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *dest, int c, size_t len)
{
	unsigned char	*dest_str;

	if (len == 0)
		return (dest);
	dest_str = (unsigned char *)dest;
	while (len--)
		*dest_str++ = (unsigned char)c;
	return (dest);
}
