/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 18:20:21 by marde-vr          #+#    #+#             */
/*   Updated: 2024/03/28 13:28:19 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cmd_is_forkable_builtin(char *cmd_token)
{
	if (!ft_strcmp(cmd_token, "echo") || !ft_strcmp(cmd_token, "ret")
		|| !ft_strcmp(cmd_token, "env") || !ft_strcmp(cmd_token, "exit")
		|| !ft_strcmp(cmd_token, "pwd"))
		return (1);
	return (0);
}

int	cmd_is_builtin(t_msh *msh, char *cmd_token)
{
	if (!cmd_token)
		return (0);
	else if (!ft_strcmp(cmd_token, "cd"))
	{
		cd(msh->cmds);
		return (1);
	}
	else if (!ft_strcmp(cmd_token, "exit"))
	{
		g_return_code = exit_bt(msh);
		return (1);
	}
	else if (!ft_strcmp(cmd_token, "export"))
	{
		if (!(/*msh->in_type == PIPE || msh->out_type == PIPE*/ 0))
			g_return_code = ft_export(msh);
		return (1);
	}
	else if (!ft_strcmp(cmd_token, "unset"))
	{
		ft_unset(msh);
		return (1);
	}
	return (cmd_is_forkable_builtin(cmd_token));
}

int	exec_builtin(t_msh *msh)
{
	if (!msh->cmds->value)
		return (0);
	if (!ft_strcmp(msh->cmds->value, "echo"))
		g_return_code = echo(msh->cmds->next);
	else if (!ft_strcmp(msh->cmds->value, "ret"))
		g_return_code = ft_atoi(msh->cmds->next->value);
	else if (!ft_strcmp(msh->cmds->value, "env"))
		g_return_code = print_env(msh->env);
	else if (!ft_strcmp(msh->cmds->value, "exit"))
		exit_bt(msh);
	else if (!ft_strcmp(msh->cmds->value, "pwd"))
		g_return_code = pwd();
	else if (!ft_strcmp(msh->cmds->value, "cd"))
		g_return_code = cd(msh->cmds);
	else
		return (0);
	return (1);
}
