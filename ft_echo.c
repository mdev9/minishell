/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomoron <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 15:30:37 by tomoron           #+#    #+#             */
/*   Updated: 2024/02/09 14:56:57 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_echo(t_cmd *args)
{
	int	put_nl;

	put_nl = 1;
	while (args && !strcmp(args->token, "-n"))
	{
		put_nl = 0;
		args = args->next;
	}
	while (args)
	{
		ft_putstr_fd(args->token, STDOUT_FILENO);
		if (args->next)
			ft_putchar_fd(' ', STDOUT_FILENO);
		args = args->next;
	}
	if (put_nl)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (0);
}
