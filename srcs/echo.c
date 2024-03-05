/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 15:30:37 by tomoron           #+#    #+#             */
/*   Updated: 2024/03/05 19:00:53 by marde-vr         ###   ########.fr       */
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

	put_nl = 1;
	while (args && args->token && !strcmp(args->token, "-n"))
	{
		put_nl = 0;
		args = args->next;
	}
	put_args(args);
	if (put_nl)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (0);
}
