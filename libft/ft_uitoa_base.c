/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_uitoa_base.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mskinner <v.golskiy@ya.ru>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/03 22:37:35 by mskinner          #+#    #+#             */
/*   Updated: 2020/09/11 02:01:32 by mskinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_uitoa_base(unsigned int n, char *base)
{
	size_t	base_len;
	size_t	len;
	size_t	i;
	char	*result;

	if ((base_len = ft_strlen(base)) < 2)
		return (NULL);
	len = ft_nbrlen_base(n, base_len);
	if (!(result = (char *)malloc(sizeof(char) * (len + 1))))
		return (NULL);
	result[len] = '\0';
	i = 0;
	while (i < len)
	{
		result[len - i - 1] = base[n % base_len];
		n /= base_len;
		i++;
	}
	return (result);
}
