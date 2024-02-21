/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_alias.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 02:54:36 by tomoron           #+#    #+#             */
/*   Updated: 2024/02/21 13:09:40 by marde-vr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*handle_alias(t_cmd *cmd, t_env *env, t_alias *alias)
{
	t_cmd	*res;
	t_cmd	*tmp;
	char	*alias_command;

	alias_command = 0;
	if (!cmd)
		return (0);
	if (cmd->type == ARG)
		alias_command = get_alias(alias, cmd->token);
	if (!alias_command)
		return (cmd);
	res = parse_command(alias_command, env);
	tmp = res;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = cmd->next;
	free(cmd);
	return (res);
}
