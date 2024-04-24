/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 15:26:01 by tomoron           #+#    #+#             */
/*   Updated: 2024/04/24 20:39:35 by marde-vr         ###   ########.fr       */
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

char	*get_token(char **cmd, int *quotes[2], t_env *env, int *is_var)
{
	char	*res;
	int		i;

	i = 0;
	while (ft_isspace(**cmd))
		(*cmd)++;
	res = ft_calloc(get_token_len(*cmd, env) + 1, 1);
	while (res && **cmd && (is_cmd_char(**cmd) || *(quotes[0]) || *(quotes[1])))
	{
		if (**cmd == '"' && !*(quotes[0]))
			*(quotes[1]) = !*(quotes[1]);
		if (**cmd == '\'' && !*(quotes[1]))
			*(quotes[0]) = !*(quotes[0]);
		if (**cmd == '$' && !*(quotes[0]))
			i += add_var_to_str(res + i, cmd, env, is_var);
		else if (**cmd == '~' && !*(quotes[0]) && !*(quotes[1]))
			i += add_home_to_str(res + i);
		else if (((**cmd == '\'' && *(quotes[1]))
				|| (**cmd == '"' && *(quotes[0])))
			|| (**cmd != '\'' && **cmd != '"'))
			res[i++] = **cmd;
		(*cmd)++;
	}
	return (res);
}

t_token	*parse_tokens(char *command, t_env *env)
{
	int		in_quote;
	int		in_dquote;
	t_token	*res;
	char	*value;
	int		is_var;

	in_quote = 0;
	in_dquote = 0;
	res = 0;
	is_var = 0;
	while (command && *command)
	{
		value = get_token(&command, (int *[2]){&in_quote, &in_dquote},
				env, &is_var);
		if (!value)
			return (free_token(res));
		res = expand_wildcards(res, value, is_var);
		while (ft_isspace(*command))
			command++;
	}
	if (command && (in_quote || in_dquote))
		return (parsing_syntax_error(res));
	return (res);
}

t_token	*parse_cmds_to_token(t_cmd *command, t_env *env)
{
	t_token	*res;
	t_token	*new;

	res = 0;
	while (command && (command->cmd_type == CMD || is_output_type(command)
			|| is_input_type(command)))
	{
		if (command->cmd_type == CMD)
		{
			new = parse_tokens(command->value, env);
			res = add_token_back(res, new);
		}
		command = command->next;
	}
	return (res);
}
