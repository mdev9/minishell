/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_var.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 15:24:36 by tomoron           #+#    #+#             */
/*   Updated: 2024/05/22 13:52:29 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_var_name(char *command)
{
	char	*res;
	int		len;

	len = get_var_name_len(command);
	res = ft_substr(command, 0, len);
	return (res);
}

int	get_var_len(char **command, t_env *env, int in_dquote)
{
	char	*var_name;
	char	*env_var;

	(*command)++;
	if ((**command == '\'' || **command == '"') && !in_dquote)
	{
		(*command)--;
		return (0);
	}
	if (!**command || (**command == '"' && in_dquote))
	{
		(*command)--;
		return (1);
	}
	if (!ft_isalnum(**command) && **command != '_' && **command != '?')
		return (2);
	if (**command == '?')
		return (get_number_len(g_return_code));
	var_name = get_var_name(*command);
	env_var = ft_get_env(env, var_name);
	free(var_name);
	if (!env_var)
		return (0);
	*command += get_var_name_len(*command) - 1;
	return (ft_strlen(env_var));
}

int	get_token_len(char *command)
{
	int	in_quote;
	int	in_dquote;
	int	res;

	in_quote = 0;
	in_dquote = 0;
	res = 0;
	while (*command && (!ft_isspace(*command) || in_quote || in_dquote))
	{
		if (*command == '"' && !in_quote)
			in_dquote = !in_dquote;
		if (*command == '\'' && !in_dquote)
			in_quote = !in_quote;
		else if (*command == '~' && !in_quote && !in_dquote)
			res += get_home_var_len();
		else if (*command != '\'' && *command != '"')
			res++;
		else if ((*command == '\'' && in_dquote) || (*command == '"'
				&& in_quote))
			res++;
		command++;
	}
	return (res);
}

int	invalid_variable_char(char *res, char c)
{
	res[0] = '$';
	res[1] = c;
	return (2);
}

int	add_var_to_str(char *res, char **command, t_env *env, int dquote)
{
	char	*var_name;
	char	*var;
	int		i;

	i = 0;
	(*command)++;
	if (**command == '\'' || **command == '"' || !**command)
	{
		if ((**command != '\'' && **command != '"') || dquote)
			*res = '$';
		(*command)--;
		return ((*(*command + 1) != '\'' && *(*command + 1) != '"') || dquote);
	}
	if (!ft_isalnum(**command) && **command != '_' && **command != '?')
		return (invalid_variable_char(res, **command));
	if (**command == '?')
		return (add_return_code_to_str(res));
	var_name = get_var_name(*command);
	var = ft_get_env(env, var_name);
	free(var_name);
	while (var && var[i++])
		res[i - 1] = var[i - 1];
	*command += get_var_name_len(*command) - 1;
	return (i - (1 - (i == 0)));
}
