/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 20:46:19 by tomoron           #+#    #+#             */
/*   Updated: 2024/02/21 23:12:34 by marde-vr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*cmd_add_back(t_cmd *cmd, char *token, t_token_type type)
{
	t_cmd	*res;
	t_cmd	*current;

	res = ft_calloc(1, sizeof(t_cmd));
	if (!res)
		return (cmd);
	res->token = token;
	res->type = type;
	if (!cmd)
		return (res);
	current = cmd;
	while (current->next)
		current = current->next;
	current->next = res;
	return (cmd);
}

void	free_cmd(t_cmd *cmd)
{
	if (cmd)
	{
		if (cmd && cmd->token)
			free(cmd->token);
		if (cmd && cmd->next)
			free_cmd(cmd->next);
		free(cmd);
	}
}
