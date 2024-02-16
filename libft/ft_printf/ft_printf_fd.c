/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 20:51:52 by marde-vr          #+#    #+#             */
/*   Updated: 2024/02/16 14:25:45 by marde-vr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"

int	putchar_fd(int fd, char c)
{
	write(fd, &c, 1);
	return (1);
}

int	ft_print_arg(int fd, va_list args_lst, char c)
{
	int	n;

	n = 0;
	if (c == 'c')
		n += putchar_fd(fd, va_arg(args_lst, int));
	else if (c == 's')
		n += putstr_fd(fd, va_arg(args_lst, char *));
	else if (c == 'p')
		n += putptr_fd(fd, va_arg(args_lst, long unsigned int));
	else if (c == 'd' || c == 'i')
		n += putnbr_fd(fd, va_arg(args_lst, int));
	else if (c == 'u')
		n += putnbr_fd(fd, va_arg(args_lst, unsigned int));
	else if (c == 'x' || c == 'X')
		n += puthexa_fd(fd, va_arg(args_lst, unsigned int), c);
	else if (c == '\0')
		return (-1);
	else
	{
		n += putchar_fd(fd, '%');
	}
	return (n);
}

int	ft_conversion_is_handled(char c)
{
	if ((c == 'c' || c == 's' || c == 'p' || c == 'd' || c == 'i'
			|| c == 'u' || c == 'x' || c == 'X' || c == '%'))
		return (1);
	return (0);
}

int	ft_printf_fd(int fd, const char *s, ...)
{
	int		n;
	int		i;
	va_list	args_lst;

	n = 0;
	i = 0;
	va_start(args_lst, s);
	while (s[i])
	{
		if (s[i] == '%' && ft_conversion_is_handled(s[i + 1]))
		{
			n += ft_print_arg(fd, args_lst, s[i + 1]);
			if (s[i + 1] != '\0')
				i += 2;
			else
				i++;
		}
		else
		{
			n += putchar_fd(fd, s[i]);
			i++;
		}
	}
	va_end(args_lst);
	return (n);
}
