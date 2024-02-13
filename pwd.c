/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 18:31:21 by tomoron           #+#    #+#             */
/*   Updated: 2024/02/13 16:18:01 by marde-vr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

int	pwd(void)
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
