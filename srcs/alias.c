/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alias.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 23:16:07 by marde-vr          #+#    #+#             */
/*   Updated: 2024/02/18 16:59:56 by marde-vr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *get_alias_name(t_cmd *arg)
{
	int i = 0;
	while (arg->token[i] != '=')
		i++;
	i++;
	char *res;
	res = ft_strdup("");
	ft_strlcpy(res, arg->token, i);
	//ft_printf("name: %s\n", res);
	return (res);
}

char *get_alias_value(t_cmd *arg)
{
	int i = 0;
	while (arg->token[i] != '=')
		i++;
	i++;
	char *res;
	res = arg->token + i;
	//ft_printf("value: %s\n", res);
	return (res);
}

int	alias(t_cmd *args, t_alias *aliases)
{
	if (ft_strchr(args->next->token, '='))
	{
		char *name = get_alias_name(args->next);
		char *value = get_alias_value(args->next);
		aliases = alias_add_back(aliases, name, value);
	}
	else
	{
		//nothin
	}
	return(0);
}
