/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unalias.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 13:11:45 by marde-vr          #+#    #+#             */
/*   Updated: 2024/02/21 17:44:30 by marde-vr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	remove_alias(t_msh *msh)
{
	t_alias	*alias;

	alias = msh->aliases;
	while (alias)
	{
		if (alias->next && msh->cmds->next
			&& !ft_strcmp(alias->next->name, msh->cmds->next->token))
		{
			if (alias->next->next)
				alias->next = alias->next->next;
			else
				alias->next = 0;
			alias->next = 0;
			free_alias(alias);
			return (1);
		}
		if (alias->next)
			alias = alias->next;
		else
			alias = 0;
	}
	return (0);
}

int	unalias(t_msh *msh)
{
	if (msh->cmds->next && !ft_strcmp(msh->cmds->next->token, "-a"))
	{
		free_alias(msh->aliases);
		msh->aliases = 0;
		return (0);
	}
	if (remove_alias(msh))
		return (1);
	if (msh->cmds->next && msh->cmds->next->type == ARG)
		ft_printf("minishell: unalias: %s: not found\n",
			msh->cmds->next->token);
	else
		ft_printf("unalias: usage: unalias name\n");
	return (1);
}
