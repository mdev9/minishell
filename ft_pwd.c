/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomoron <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 18:31:21 by tomoron           #+#    #+#             */
/*   Updated: 2024/02/12 14:53:05 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

int	ft_pwd(void)
{
	char	*buffer;

	buffer = malloc(1024 * 1024);
	if (!buffer)
		return (1);
	getcwd(buffer, 1024 * 1024);
	ft_printf("%s\n", buffer);
	free(buffer);
	return (0);
}
