/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 15:30:37 by tomoron           #+#    #+#             */
/*   Updated: 2024/03/26 14:14:29 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

void	put_args(t_cmd *args)
{
	int	first;

	first = 1;
	while (args && args->type != PIPE)
	{
		if (args->type != ARG)
			args = args->next;
		else
		{
			if (!first)
				ft_putchar_fd(' ', STDOUT_FILENO);
			ft_putstr_fd(args->token, STDOUT_FILENO);
			first = 0;
		}
		args = args->next;
	}
}

int	echo(t_cmd *args)
{
	int	put_nl;
	int	i;

	put_nl = 1;
	i = 1;
	while (args && args->token && args->token[0] == '-')
	{
		while (args->token[i] == 'n')
			i++;
		if (!args->token[i])
			put_nl = 0;
		else
			ft_printf("%s ",args->token);
		args = args->next;
	}
	put_args(args);
	if (put_nl)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (0);
}
