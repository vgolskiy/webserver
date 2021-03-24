/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nbrlen_base.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mskinner <v.golskiy@ya.ru>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/07 18:35:26 by mskinner          #+#    #+#             */
/*   Updated: 2020/09/11 01:56:19 by mskinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_nbrlen_base(long long n, long long base_len)
{
	int	len;

	len = 1;
	if (n < 0)
		n *= -1;
	while (n >= base_len)
	{
		n /= base_len;
		len++;
	}
	return (len);
}
