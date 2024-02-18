/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alias.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 23:16:07 by marde-vr          #+#    #+#             */
/*   Updated: 2024/02/18 19:05:38 by marde-vr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_alias_name(t_cmd *arg)
{
	int		i;
	char	*res;

	i = 0;
	while (arg->token[i] && arg->token[i] != '=')
		i++;
	i++;
	res = ft_calloc(i, 1);
	ft_strlcpy(res, arg->token, i);
	return (res);
}

char	*get_alias_value(t_cmd *arg)
{
	int		i;
	char	*res;

	i = 0;
	while (arg->token[i] != '=')
		i++;
	i++;
	res = ft_strdup(arg->token + i);
	return (res);
}

int	unalias(t_cmd *args, t_alias **aliases)
{
	t_alias	*alias;

	if (args->next && !ft_strcmp(args->next->token, "-a"))
	{
		free_alias(*aliases);
		*aliases = 0;
		return (0);
	}
	alias = *aliases;
	while (alias)
	{
		if (alias->next && args->next && !ft_strcmp(alias->next->name, args->next->token))
		{
			if (alias->next->next)
				alias->next = alias->next->next;
			else
				alias->next = 0;
			alias->next = 0;
			free_alias(alias);
			return (0);
		}
		if (alias->next)
			alias = alias->next;
		else
			alias = 0;
	}
	if (args->next && args->next->type == ARG)
		ft_printf("minishell: unalias: %s: not found\n", args->next->token);
	else
		ft_printf("unalias: usage: unalias name\n");
	return (1);
}

int	alias(t_cmd *args, t_alias **aliases)
{
	char	*name;
	char	*value;
	t_alias	*cur_alias;

	cur_alias = *aliases;
	if (!args->next || args->next->type != ARG)
	{
		while (cur_alias)
		{
			ft_printf("alias %s=%s\n", cur_alias->name, cur_alias->value);
			cur_alias = cur_alias->next;
		}
	}
	else
	{
		name = get_alias_name(args->next);
		if (ft_strchr(args->next->token, '='))
		{
			value = get_alias_value(args->next);
			*aliases = alias_add_back(*aliases, name, value);
		}
		else
		{
			ft_printf("alias %s=%s\n", name, ft_get_alias(*aliases, name));
			free(name);
		}
	}
	return (0);
}
