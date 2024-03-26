/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 15:30:37 by tomoron           #+#    #+#             */
/*   Updated: 2024/03/26 08:43:16 by marde-vr         ###   ########.fr       */
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
		if (put_nl)
			put_args(args);
		args = args->next;
	}
	if (!put_nl)
		put_args(args);
	if (put_nl)
	{
		if (!(args->token[0] == '-'))
			put_args(args);
		ft_putchar_fd('\n', STDOUT_FILENO);
	}
	return (0);
}
