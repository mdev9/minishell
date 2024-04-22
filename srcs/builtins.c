/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 18:20:21 by marde-vr          #+#    #+#             */
/*   Updated: 2024/04/22 19:39:03 by marde-vr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cmd_is_forkable_builtin(char *cmd_token)
{
	if (!ft_strcmp(cmd_token, "echo") || !ft_strcmp(cmd_token, "ret")
		|| !ft_strcmp(cmd_token, "env") || !ft_strcmp(cmd_token, "exit")
		|| !ft_strcmp(cmd_token, "pwd") || !ft_strcmp(cmd_token, "cd")
		|| !ft_strcmp(cmd_token, "export") || !ft_strcmp(cmd_token, "unset"))
		return (1);
	return (0);
}

int	cmd_is_builtin(t_msh *msh, char *cmd_token)
{
	if (!cmd_token)
		return (0);
	else if ((msh->in_type == PIPE || msh->out_type == PIPE)
		&& cmd_is_forkable_builtin(cmd_token))
		return (1);
	else if (!ft_strcmp(cmd_token, "cd"))
		g_return_code = cd(msh->tokens);
	else if (!ft_strcmp(cmd_token, "exit"))
		g_return_code = exit_bt(msh);
	else if (!ft_strcmp(cmd_token, "export"))
		g_return_code = ft_export(msh->tokens, msh->env);
	else if (!ft_strcmp(cmd_token, "unset"))
		g_return_code = ft_unset(msh);
	else
		return (cmd_is_forkable_builtin(cmd_token));
	return (1);
}

int	exec_builtin(t_msh *msh)
{
	if (!msh->tokens->value)
		return (0);
	if (!ft_strcmp(msh->tokens->value, "echo"))
		g_return_code = echo(msh->tokens->next);
	else if (!ft_strcmp(msh->tokens->value, "ret"))
		g_return_code = ft_atoi(msh->tokens->next->value);
	else if (!ft_strcmp(msh->tokens->value, "env"))
		g_return_code = print_env(msh->env);
	else if (!ft_strcmp(msh->tokens->value, "exit"))
		return (1);
	else if (!ft_strcmp(msh->tokens->value, "pwd"))
		g_return_code = pwd();
	else if (!ft_strcmp(msh->tokens->value, "cd"))
		return (1);
	else if (!ft_strcmp(msh->tokens->value, "export"))
		return (1);
	else if (!ft_strcmp(msh->tokens->value, "unset"))
		return (1);
	else
		return (0);
	return (1);
}
