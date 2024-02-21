/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alias.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 23:16:07 by marde-vr          #+#    #+#             */
/*   Updated: 2024/02/21 13:12:07 by marde-vr         ###   ########.fr       */
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

void	print_aliases(t_alias **aliases)
{
	t_alias	*cur_alias;

	cur_alias = *aliases;
	while (cur_alias)
	{
		ft_printf("alias %s=%s\n", cur_alias->name, cur_alias->value);
		cur_alias = cur_alias->next;
	}
}

int	alias(t_cmd *args, t_alias **aliases)
{
	char	*name;
	char	*value;

	if (!args->next || args->next->type != ARG)
		print_aliases(aliases);
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
			ft_printf("alias %s=%s\n", name, get_alias(*aliases, name));
			free(name);
		}
	}
	return (0);
}
