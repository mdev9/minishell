/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing_var.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomoron <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 15:24:36 by tomoron           #+#    #+#             */
/*   Updated: 2024/02/09 15:24:43 by tomoron          ###   ########.fr       */
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

int	ft_get_var_len(char **command, t_env *env)
{
	char	*var_name;
	char	*env_var;

	(*command)++;
	if (!ft_isalnum(**command) && **command != '_' && **command != '?')
		return (1);
	if (**command == '?')
		return (get_number_len(g_return_code));
	var_name = get_var_name(*command);
	env_var = ft_getenv(env, var_name);
	free(var_name);
	if (!env_var)
		return (0);
	*command += get_var_name_len(*command) - 1;
	return (ft_strlen(env_var));
}

int	ft_get_token_len(char *command, t_env *env)
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
		if (*command == '$' && !in_quote)
			res += ft_get_var_len(&command, env);
		else if (*command != '\'' && *command != '"')
			res++;
		else if ((*command == '\'' && in_dquote)
			|| (*command == '"' && in_quote))
