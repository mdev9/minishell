/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 16:04:11 by tomoron           #+#    #+#             */
/*   Updated: 2024/03/06 09:10:11 by marde-vr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	numeric_arg_err(char *arg, int *exit_code)
{
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(": numeric argument required\n", 2);
	*exit_code = 2;
}

void	exit_bt(t_msh *msh)
{
	t_cmd	*cur_cmd;	
	int		exit_code;

	cur_cmd = msh->cmds->next;
	ft_printf("exit\n");
	if (cur_cmd && cur_cmd->next && cur_cmd->next->type == ARG
		&& ft_strisnbr(cur_cmd->token))
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		g_return_code = 1;
	}
	else
	{
		if (cur_cmd && cur_cmd->type == ARG
			&& !ft_strisnbr(cur_cmd->token))
			numeric_arg_err(cur_cmd->token, &exit_code);
		else if (cur_cmd && cur_cmd->type == ARG)
			exit_code = (unsigned char)ft_atoi(cur_cmd->token);
		else
			exit_code = g_return_code;
		free_msh(msh);
		exit(exit_code);
	}
}
