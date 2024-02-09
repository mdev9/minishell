/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomoron <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 16:04:11 by tomoron           #+#    #+#             */
/*   Updated: 2024/02/09 15:08:01 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_exit(t_cmd *args, t_env *env)
{
	t_cmd	*start;	
	int		exit_code;

	start = args;
	args = args->next;
	ft_printf("exit\n");
	if (args && args->next)
		ft_printf("minishell: exit: too many arguments\n");
	else
	{
		if (args)
			exit_code = (unsigned char)ft_atoi(args->token);
		else
			exit_code = g_return_code;
		ft_free_cmd(start);
		ft_free_env(env);
		exit(g_return_code);
	}
}
