/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_alias.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 20:46:19 by tomoron           #+#    #+#             */
/*   Updated: 2024/02/17 04:34:24 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_alias	*alias_add_back(t_alias *alias, char *name, char *value)
{
	t_alias	*res;
	t_alias	*current;

	res = ft_calloc(1, sizeof(t_alias));
	if (!res)
		return (alias);
	res->name = name;
	res->value = value;
	if (!alias)
		return (res);
	current = alias;
	while (current->next)
		current = current->next;
	current->next = res;
	return (alias);
}

void	free_alias(t_alias *alias)
{
	if (alias && alias->next)
		free_alias(alias->next);
	if (alias)
	{
		free(alias->name);
		free(alias->value);
	}
	free(alias);
}

char	*ft_get_alias(t_alias *alias, char *name)
{
	while (alias)
	{
		if (!ft_strcmp(alias->name, name))
			return (alias->value);
		alias = alias->next;
	}
	return (0);
}


