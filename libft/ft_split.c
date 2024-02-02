/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomoron <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 10:30:03 by tomoron           #+#    #+#             */
/*   Updated: 2023/11/01 15:18:57 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_count_parts(char *str, char charset)
{
	int	res;

	res = 0;
	while (*str == charset && *str)
		str++;
	while (*str)
	{
		while (!(*str == charset) && *str)
			str++;
		while (*str == charset && *str)
			str++;
		res++;
	}
	return (res + 1);
}

static int	ft_strlen_sep(char *str, char charset)
{
	int	res;

	res = 0;
	while (*str && !(*str == charset))
	{
		res++;
		str++;
	}
	return (res + 1);
}

char	**ft_split(const char *str, char charset)
{
	int		nb_str;
	char	**res;
	int		i;
	int		j;

	if (!str)
		return (0);
	nb_str = ft_count_parts((char *)str, charset);
	res = (char **)malloc(nb_str * sizeof(char *));
	i = -1;
	while (res && *str && *str == charset)
		str++;
	while (++i < nb_str - 1 && *str && res)
	{
		res[i] = malloc(ft_strlen_sep((char *)str, charset));
		j = 0;
		while (res[i] && *str && !(*str == charset))
			res[i][j++] = *(str++);
		res[i][j] = 0;
		while (*str == charset)
			str++;
	}
	if (res)
		res[i] = 0;
	return (res);
}
