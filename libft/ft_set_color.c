/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_set_color.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomoron <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 10:56:11 by tomoron           #+#    #+#             */
/*   Updated: 2024/02/04 02:00:50 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_set_color(int r, int g, int b)
{
	ft_printf("\033[38;2;%d;%d;%dm", r, g, b);
}

char	*ft_get_color(int r, int g, int b)
{
	char	*res;

	res = ft_strjoin_free("\033[38;2;", ft_itoa(r), 2);
	res = ft_strjoin_free(res, ";", 1);
	res = ft_strjoin_free(res, ft_itoa(g), 3);
	res = ft_strjoin_free(res, ";", 1);
	res = ft_strjoin_free(res, ft_itoa(b), 3);
	res = ft_strjoin_free(res, "m", 1);
	return (res);
}

void	ft_reset_color(void)
{
	ft_printf("\033[0m");
}
