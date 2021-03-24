/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mskinner <v.golskiy@ya.ru>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/24 14:26:10 by mskinner          #+#    #+#             */
/*   Updated: 2020/11/14 21:18:17 by mskinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_rewind(const char *str)
{
	int i;

	i = 0;
	while (str[i] == '\t' || str[i] == '\v' || str[i] == '\f' ||
		str[i] == '\n' || str[i] == ' ' || str[i] == '\r')
		i++;
	return ((char *)str + i);
}

static int	ft_get_num(const char *str, int sign)
{
	int			i;
	long int	tmp;

	i = 0;
	tmp = 0;
	while (str[i] != '\0')
	{
		if (ft_isdigit(str[i]))
		{
			tmp = tmp * 10 + str[i] - '0';
			if (tmp < 0)
			{
				if (sign < 0)
					return (0);
				return (-1);
			}
			i++;
		}
		else
			break ;
	}
	return (tmp);
}

int			ft_atoi(const char *str)
{
	int	sign;

	sign = 1;
	str = ft_rewind(str);
	if (*str == '-')
		sign = -1;
	if ((*str == '-') || (*str == '+'))
		str++;
	return (sign * ft_get_num(str, sign));
}
