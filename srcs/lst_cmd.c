/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 20:46:19 by tomoron           #+#    #+#             */
/*   Updated: 2024/04/18 20:48:55 by marde-vr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*cmd_add_back(t_cmd *cmd, char *value, t_cmd_type type)
{
	t_cmd	*res;
	t_cmd	*current;

	if (value && !*value)
	{
		free(value);
		return (cmd);
	}
	res = ft_calloc(1, sizeof(t_cmd));
	if (!res)
		return (cmd);
	res->value = value;
	res->cmd_type = type;
	if (!cmd)
		return (res);
	current = cmd;
	while (current->next)
		current = current->next;
	current->next = res;
	return (cmd);
}

t_cmd	*free_cmd(t_cmd *cmd)
{
	if (cmd)
	{
		if (cmd && cmd->value)
		{
			free(cmd->value);
			cmd->value = 0;
		}
		if (cmd && cmd->next)
		{
			free_cmd(cmd->next);
			cmd->next = 0;
		}
		free(cmd);
		cmd = 0;
	}
	return (0);
}
