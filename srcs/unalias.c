/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unalias.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 13:11:45 by marde-vr          #+#    #+#             */
/*   Updated: 2024/02/21 13:11:59 by marde-vr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	remove_alias(t_cmd *args, t_alias **aliases)
{
	t_alias	*alias;

	alias = *aliases;
	while (alias)
	{
		if (alias->next && args->next
			&& !ft_strcmp(alias->next->name, args->next->token))
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

int	unalias(t_cmd *args, t_alias **aliases)
{
	if (args->next && !ft_strcmp(args->next->token, "-a"))
	{
		free_alias(*aliases);
		*aliases = 0;
		return (0);
	}
	if (remove_alias(args, aliases))
		return (1);
	if (args->next && args->next->type == ARG)
		ft_printf("minishell: unalias: %s: not found\n", args->next->token);
	else
		ft_printf("unalias: usage: unalias name\n");
	return (1);
}
