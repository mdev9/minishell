/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 15:26:01 by tomoron           #+#    #+#             */
/*   Updated: 2024/04/15 15:56:17 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

int	is_cmd_char(char c)
{
	return (!ft_isspace(c) && c != '|' && c != '&' && c != '<' && c != '>');
}

int	add_home_to_str(char *res)
{
	int		i;
	char	*str;

	i = 0;
	str = getenv("HOME");
	while (str[i])
	{
		res[i] = str[i];
		i++;
	}
	return (i);
}

char	*get_token(char **cmd, int *in_quote, int *in_dquote, t_env *env)
{
	char	*res;
	int		i;

	i = 0;
	while (ft_isspace(**cmd))
		(*cmd)++;
	res = ft_calloc(get_token_len(*cmd, env) + 1, 1);
	while (res && **cmd && (is_cmd_char(**cmd) || *in_quote || *in_dquote))
	{
		if (**cmd == '"' && !*in_quote)
			*in_dquote = !*in_dquote;
		if (**cmd == '\'' && !*in_dquote)
			*in_quote = !*in_quote;
		if (**cmd == '$' && !*in_quote)
			i += add_var_to_str(res + i, cmd, env);
		else if (**cmd == '~' && !*in_quote && !*in_dquote)
			i += add_home_to_str(res + i);
		else if (((**cmd == '\'' && *in_dquote) || (**cmd == '"' && *in_quote))
					|| (**cmd != '\'' && **cmd != '"'))
			res[i++] = **cmd;
		(*cmd)++;
	}
	return (res);
}

t_token	*parse_command(char *command, t_env *env)
{
	int				in_quote;
	int				in_dquote;
	t_token			*res;
	char			*value;

	in_quote = 0;
	in_dquote = 0;
	res = 0;
	while (command && *command)
	{
		value = get_token(&command, &in_quote, &in_dquote, env);
		if (!value)
			return (free_token(res));
		res = expand_wildcards(res, value);
		while (ft_isspace(*command))
			command++;
	}
	if (command && (in_quote || in_dquote))
		return (parsing_syntax_error(res));
	return (res);
}
