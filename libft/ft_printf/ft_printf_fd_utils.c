/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_fd_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 21:06:24 by marde-vr          #+#    #+#             */
/*   Updated: 2024/02/16 14:28:23 by marde-vr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	putstr_fd(int fd, const char *s)
{
	int	i;

	i = 0;
	if (!s)
	{
		write(fd, &"(null)", 6);
		return (6);
	}
	while (s[i])
	{
		write(fd, &s[i], 1);
		i++;
	}
	return (i);
}

int	putnbr_fd(int fd, long nb)
{
	int	n;

	n = 0;
	if (nb < 0)
	{
		write(fd, "-", 1);
		nb = nb * -1;
		n++;
	}
	if (nb > 9)
	{
		n += putnbr_fd(fd, nb / 10);
	}
	write(fd, &(char){nb % 10 + 48}, 1);
	return (n + 1);
}

int	puthexa_ptr_fd(int fd, unsigned long nbr)
{
	int	n;

	n = 0;
	if (nbr > 15)
		n += puthexa_ptr_fd(fd, nbr / 16);
	write(fd, &"0123456789abcdef"[nbr % 16], 1);
	return (n + 1);
}

int	putptr_fd(int fd, long unsigned int ptr)
{
	int	n;

	if (!ptr)
	{
		write(fd, &"(nil)", 5);
		return (5);
	}
	putstr_fd(fd, "0x");
	n = 2;
	n += puthexa_ptr_fd(fd, ptr);
	return (n);
}

int	puthexa_fd(int fd, unsigned int nbr, char c)
{
	int	n;

	n = 0;
	if (nbr > 15)
		n += puthexa_fd(fd, nbr / 16, c);
	if (c == 'X')
		write(fd, &"0123456789ABCDEF"[nbr % 16], 1);
	if (c == 'x')
		write(fd, &"0123456789abcdef"[nbr % 16], 1);
	return (n + 1);
}
