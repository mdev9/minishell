/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 15:26:01 by tomoron           #+#    #+#             */
/*   Updated: 2024/03/29 14:31:04 by tomoron          ###   ########.fr       */
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

t_token_type	get_token_type(char **command)
{
	t_token_type	res;

	while (ft_isspace(**command))
		(*command)++;
	if ((*command)[0] == '>' && (*command)[1] == '>')
		res = RED_O_APP;
	else if ((*command)[0] == '<' && (*command)[1] == '<')
		res = HERE_DOC;
	else if ((*command)[0] == '>')
		res = RED_O;
	else if ((*command)[0] == '<')
		res = RED_I;
	else
		res = ARG;
	if (res == RED_O_APP || res == HERE_DOC)
		(*command) += 2;
	if (res == RED_O || res == RED_I)
		(*command)++;
	return (res);
}

t_token	*parse_command(char *command, t_env *env)
{
	int				in_quote;
	int				in_dquote;
	t_token			*res;
	char			*value;
	t_token_type	type;

	in_quote = 0;
	in_dquote = 0;
	res = 0;
	while (command && *command)
	{
		type = get_token_type(&command);
		if (type == ARG)
			value = get_token(&command, &in_quote, &in_dquote, env);
		else
			value = 0;
		if (type == ARG && value == 0)
			return (free_token(res));
		res = token_add_back(res, value, type);
		while (ft_isspace(*command))
			command++;
	}
	if (command && (in_quote || in_dquote))
		return (parsing_syntax_error(res));
	return (res);
}
