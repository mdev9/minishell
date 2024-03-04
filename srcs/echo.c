/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 15:30:37 by tomoron           #+#    #+#             */
/*   Updated: 2024/03/04 11:17:02 by marde-vr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

int	echo(t_cmd *args)
{
	int	put_nl;

	put_nl = 1;
	while (args && args->token && !strcmp(args->token, "-n"))
	{
		put_nl = 0;
		args = args->next;
	}
	while (args && args->type !=PIPE)
	{
		if (args->type != ARG)
			args = args->next;
		else
		{
			ft_putstr_fd(args->token, STDOUT_FILENO);
			if (args->next)
			{
				ft_printf_fd(2, "yes\n");
				ft_putchar_fd(' ', STDOUT_FILENO);
			}
		}
		args = args->next;
	}
	if (put_nl)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (0);
}
