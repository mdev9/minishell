/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 20:46:19 by tomoron           #+#    #+#             */
/*   Updated: 2024/03/27 17:24:55 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*cmd_add_back(t_token *cmd, char *value, t_token_type type)
{
	t_token	*res;
	t_token	*current;

	if (value && !*value)
	{
		free(value);
		return (cmd);
	}
	res = ft_calloc(1, sizeof(t_token));
	if (!res)
		return (cmd);
	res->value = value;
	res->type = type;
	if (!cmd)
		return (res);
	current = cmd;
	while (current->next)
		current = current->next;
	current->next = res;
	return (cmd);
}

t_token	*free_cmd(t_token *cmd)
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
