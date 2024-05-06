/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expantion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomoron <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 10:57:33 by tomoron           #+#    #+#             */
/*   Updated: 2024/05/06 10:57:39 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

int	get_variable_expantion_len(char *command, t_env *env)
{
	int	in_quote;
	int	in_dquote;
	int	i;

	i = 0;
	in_dquote = 0;
	in_quote = 0;
	while (*command)
	{
		if (*command == '\'' && !in_dquote)
			in_quote = !in_quote;
		if (*command == '"' && !in_quote)
			in_dquote = !in_dquote;
		if (*command == '$' && !in_quote)
			i += get_var_len(&command, env, in_dquote);
		else
			i++;
		command++;
	}
	return (i);
}

char	*expand_variables(char *command, t_env *env, int *is_var)
{
	char	*res;
	int		i;
	int		in_dquote;
	int		in_quote;

	if (!command)
		return (0);
	res = ft_calloc(get_variable_expantion_len(command, env) + 1, 1);
	in_quote = 0;
	in_dquote = 0;
	*is_var = 1;
	i = 0;
	while (res && *command)
	{
		if (*command == '\'' && !in_dquote)
			in_quote = !in_quote;
		if (*command == '"' && !in_quote)
			in_dquote = !in_dquote;
		if (*command == '$' && !in_quote)
			i += add_var_to_str(res + i, &command, env, in_dquote);
		else
			res[i++] = *command;
		command++;
	}
	return (res);
}
