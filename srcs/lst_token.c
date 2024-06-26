/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_token.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 20:46:19 by tomoron           #+#    #+#             */
/*   Updated: 2024/04/24 18:04:15 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*token_add_back(t_token *token, char *value, int is_var)
{
	t_token	*res;
	t_token	*current;

	if (value && !*value && is_var)
	{
		free(value);
		return (token);
	}
	res = ft_calloc(1, sizeof(t_token));
	if (!res)
		return (token);
	res->value = value;
	if (!token)
		return (res);
	current = token;
	while (current->next)
		current = current->next;
	current->next = res;
	return (token);
}

t_token	*free_token(t_token *token)
{
	if (token)
	{
		if (token && token->value)
		{
			free(token->value);
			token->value = 0;
		}
		if (token && token->next)
		{
			free_token(token->next);
			token->next = 0;
		}
		free(token);
		token = 0;
	}
	return (0);
}
