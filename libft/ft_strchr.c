/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 13:35:44 by tomoron           #+#    #+#             */
/*   Updated: 2024/03/21 16:41:59 by marde-vr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	int		i;
	char	*res;

	i = 0;
	if (!s)
		return (NULL);
	res = (char *)s;
	while (res[i])
	{
		if (res[i] == (unsigned char)c)
			return (res + i);
		i++;
	}
	if (res[i] == (unsigned char)c)
		return (res + i);
	return (NULL);
}
