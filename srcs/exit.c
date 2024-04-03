/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 16:04:11 by tomoron           #+#    #+#             */
/*   Updated: 2024/04/03 15:44:06 by tomoron          ###   ########.fr       */
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

void	get_exit_bt_return_code(t_msh *msh, int *exit_code)
{
	t_token	*cur_cmd;

	cur_cmd = msh->tokens->next;
	if (cur_cmd && !ft_strisnbr(cur_cmd->value))
		numeric_arg_err(cur_cmd->value, exit_code);
	else if (cur_cmd)
		*exit_code = (unsigned char)ft_atoi(cur_cmd->value);
	else
		*exit_code = g_return_code;
}

int	exit_bt(t_msh *msh)
{
	t_token	*cur_cmd;
	int		exit_code;

	cur_cmd = msh->tokens->next;
	ft_printf("exit\n");
	if (cur_cmd && cur_cmd->next && ft_strisnbr(cur_cmd->value))
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
	else
	{
		get_exit_bt_return_code(msh, &exit_code);
		free_msh(msh);
		exit(exit_code);
	}
	return (1);
}
