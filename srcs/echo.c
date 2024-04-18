/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 15:30:37 by tomoron           #+#    #+#             */
/*   Updated: 2024/04/18 20:48:52 by marde-vr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

void	put_args(t_token *args)
{
	int	first;

	first = 1;
	while (args)
	{
		if (!first)
			ft_putchar_fd(' ', STDOUT_FILENO);
		ft_putstr_fd(args->value, STDOUT_FILENO);
		first = 0;
		args = args->next;
	}
}

int	echo(t_token *args)
{
	int	put_nl;
	int	i;

	put_nl = 1;
	i = 1;
	while (args && args->value && args->value[0] == '-')
	{
		while (args->value[i] == 'n')
			i++;
		if (!args->value[i])
			put_nl = 0;
		else
			ft_printf("%s ", args->value);
		args = args->next;
	}
	put_args(args);
	if (put_nl)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (0);
}
