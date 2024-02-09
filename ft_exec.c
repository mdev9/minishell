/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomoron <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 14:12:49 by tomoron           #+#    #+#             */
/*   Updated: 2024/02/09 14:59:22 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_builtin(t_cmd *parsed_cmd, t_env *env)
{
	if (!strcmp(parsed_cmd->token, "echo"))
		g_return_code = ft_echo(parsed_cmd->next);
	else if (!strcmp(parsed_cmd->token, "ret"))
		g_return_code = ft_atoi(parsed_cmd->next->token);
	else if (!strcmp(parsed_cmd->token, "env"))
		g_return_code = ft_print_env(env);
	else if (!strcmp(parsed_cmd->token, "exit"))
		ft_exit(parsed_cmd, env);
	else
		return (STDIN_FILENO);
	return (STDOUT_FILENO);
}

void	ft_exec_command(t_cmd *parsed_cmd, t_env *env)
{
	if (!parsed_cmd)
		return ;
	if (exec_builtin(parsed_cmd, env))
		return ;
	ft_printf("not a builtin\n");
}
