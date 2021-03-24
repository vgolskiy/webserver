/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_base.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mskinner <v.golskiy@ya.ru>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/10 19:12:19 by mskinner          #+#    #+#             */
/*   Updated: 2020/09/11 01:57:55 by mskinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_base(int n, char *str)
{
	int	radix;

	radix = ft_strlen(str);
	if (!radix)
		return ;
	if (n >= 0)
	{
		if (n > 9)
			ft_putnbr_base(n / radix, str);
		ft_putchar(str[n % radix]);
	}
	else
	{
		ft_putchar('-');
		if (n < -9)
			ft_putnbr_base(n / (radix * (-1)), str);
		ft_putchar(str[n % radix * -1]);
	}
}
