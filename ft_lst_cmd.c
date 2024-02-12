/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lst_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomoron <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 20:46:19 by tomoron           #+#    #+#             */
/*   Updated: 2024/02/11 22:54:28 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*ft_cmd_add_back(t_cmd *cmd, char *token, t_token_type type)
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

void	ft_free_cmd(t_cmd *cmd)
{
	if (cmd && cmd->next)
		ft_free_cmd(cmd->next);
	if (cmd)
		free(cmd->token);
	free(cmd);
}
