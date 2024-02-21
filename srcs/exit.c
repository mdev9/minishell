/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 16:04:11 by tomoron           #+#    #+#             */
/*   Updated: 2024/02/21 17:43:52 by marde-vr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_numeric_arg_err(char *arg)
{
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(": numeric argument required\n", 2);
}

void	exit_bt(t_msh *msh)
{
	t_cmd	*start;	
	int		exit_code;

	start = msh->cmds;
	msh->cmds = msh->cmds->next;
	ft_printf("exit\n");
	if (msh->cmds && msh->cmds->next && msh->cmds->next->type == ARG
		&& ft_strisnbr(msh->cmds->token))
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
	else
	{
		if (msh->cmds && msh->cmds->type == ARG
			&& !ft_strisnbr(msh->cmds->token))
			print_numeric_arg_err(msh->cmds->token);
		if (msh->cmds && msh->cmds->type == ARG)
			exit_code = (unsigned char)ft_atoi(msh->cmds->token);
		else if (msh->cmds && msh->cmds->type == ARG
			&& !ft_strisnbr(msh->cmds->token))
			exit_code = 2;
		else
			exit_code = g_return_code;
		free_cmd(start);
		free_msh(msh);
		exit(exit_code);
	}
}
