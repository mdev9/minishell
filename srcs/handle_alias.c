/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_alias.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 02:54:36 by tomoron           #+#    #+#             */
/*   Updated: 2024/02/21 17:21:27 by marde-vr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*handle_alias(t_msh *msh)
{
	t_cmd	*res;
	t_cmd	*tmp;
	char	*alias_command;

	alias_command = 0;
	if (!msh->cmds)
		return (0);
	if (msh->cmds->type == ARG)
		alias_command = get_alias(msh->aliases, msh->cmds->token);
	if (!alias_command)
		return (msh->cmds);
	res = parse_command(alias_command, msh->env);
	tmp = res;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = msh->cmds->next;
	free(msh->cmds);
	return (res);
}
