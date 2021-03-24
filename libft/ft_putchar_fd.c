/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mskinner <v.golskiy@ya.ru>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 14:21:07 by mskinner          #+#    #+#             */
/*   Updated: 2020/10/14 18:58:17 by mskinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

ssize_t	ft_putchar_fd(wchar_t c, int fd)
{
	if (!fd)
		return (0);
	if (c > 0x80)
	{
		return (pwchar(c, fd));
	}
	return (write(fd, &c, 1));
}
