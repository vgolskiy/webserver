/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mskinner <v.golskiy@ya.ru>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/24 14:26:43 by mskinner          #+#    #+#             */
/*   Updated: 2020/09/11 01:50:54 by mskinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	unsigned char	*new;

	if (!(new = (unsigned char *)malloc(size * nmemb)))
		return (NULL);
	ft_memset(new, 0, nmemb * size);
	return ((void *)new);
}
