/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomoron <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 16:04:11 by tomoron           #+#    #+#             */
/*   Updated: 2024/02/12 20:11:51 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_print_numeric_arg_err(char *arg)
{
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(": numeric argument required\n", 2);
}

void	ft_exit(t_cmd *args, t_env *env)
{
	t_cmd	*start;	
	int		exit_code;

	start = args;
	args = args->next;
	ft_printf("exit\n");
	if (args && args->next && ft_strisnbr(args->token))
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
	else
	{
		if (args && !ft_strisnbr(args->token))
			ft_print_numeric_arg_err(args->token);
		if (args)
			exit_code = (unsigned char)ft_atoi(args->token);
		else if (args && !ft_strisnbr(args->token))
			exit_code = 2;
		else
			exit_code = g_return_code;
		ft_free_cmd(start);
		ft_free_env(env);
		exit(exit_code);
	}
}
