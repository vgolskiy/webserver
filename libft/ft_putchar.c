/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mskinner <v.golskiy@ya.ru>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/10 18:24:48 by mskinner          #+#    #+#             */
/*   Updated: 2020/10/14 18:57:30 by mskinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static ssize_t	pwchar2(wchar_t c, char *t)
{
	t[0] = (char)((c >> 6) | 0xc0);
	t[1] = (char)((c & 0x3f) | 0x80);
	return (2);
}

static ssize_t	pwchar3(wchar_t c, char *t)
{
	t[0] = (char)((c >> 12) | 0xc0);
	t[1] = (char)(((c >> 6 & 0x3f)) | 0x80);
	t[2] = (char)((c & 0x3f) | 0x80);
	return (3);
}

static ssize_t	pwchar4(wchar_t c, char *t)
{
	t[0] = (char)((c >> 18) | 0xc0);
	t[1] = (char)(((c >> 12) & 0x3f) | 0x80);
	t[2] = (char)(((c >> 6) & 0x3f) | 0x80);
	t[3] = (char)((c & 0x3f) | 0x80);
	return (4);
}

ssize_t			pwchar(wchar_t c, int fd)
{
	unsigned int	i;
	char			t[4];

	i = -1;
	while (++i < 4)
		t[i] = '\0';
	if (c < 0x8000)
		i = pwchar2(c, t);
	else if (c < 0x80000)
		i = pwchar3(c, t);
	else
		i = pwchar4(c, t);
	return (write(fd, t, i));
}

ssize_t			ft_putchar(wchar_t c)
{
	if (c > 0x80)
	{
		return (pwchar(c, 1));
	}
	return (write(1, &c, 1));
}
