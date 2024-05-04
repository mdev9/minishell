/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 15:26:01 by tomoron           #+#    #+#             */
/*   Updated: 2024/05/04 14:30:22 by tomoron          ###   ########.fr       */
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

char	*get_token(char **cmd, int quotes[2])
{
	char	*res;
	int		i;

	i = 0;
	while (ft_isspace(**cmd))
		(*cmd)++;
	res = ft_calloc(get_token_len(*cmd) + 1, 1);
	while (res && **cmd && (is_cmd_char(**cmd) || quotes[0] || quotes[1]))
	{
		if (**cmd == '"' && !quotes[0])
			quotes[1] = !quotes[1];
		if (**cmd == '\'' && !quotes[1])
			quotes[0] = !quotes[0];
		else if (**cmd == '~' && !quotes[0] && !quotes[1])
			i += add_home_to_str(res + i);
		else if (((**cmd == '\'' && quotes[1])
				|| (**cmd == '"' && quotes[0]))
			|| (**cmd != '\'' && **cmd != '"'))
			res[i++] = **cmd;
		(*cmd)++;
	}
	return (res);
}

int	get_variable_expantion_len(char *command , t_env *env)
{
	int in_quote;
	int	in_dquote;
	int i;

	i = 0;
	in_dquote = 0;
	in_quote = 0;
	while(*command)
	{
		if(*command == '\'' && !in_dquote)
			in_quote = !in_quote;
		if(*command == '"' && !in_quote)
			in_dquote = !in_dquote;
		if(*command == '$' && !in_quote)
			i+= get_var_len(&command, env);
		else
			i++;
		command++;
	}
	return(i);
}

char	*expand_variables(char *command, t_env *env)
{
	char *res;
	char *start;
	int i;
	int in_dquote;
	int in_quote;

	if(!command)
		return(0);
	res = ft_calloc(get_variable_expantion_len(command, env) + 1, 1);
	start = command;
	in_quote = 0;
	in_dquote = 0;
	i = 0;
	while(res && *command)
	{
		if(*command == '\'' && !in_dquote)
			in_quote = !in_quote;
		if(*command == '"' && !in_quote)
			in_dquote = !in_dquote;
		if(*command == '$' && !in_quote)
			i+= add_var_to_str(res + i, &command ,env);
		else
			res[i++] = *command;
		command++;
	}
	return(res);
}

t_token	*parse_tokens(char *command, t_env *env)
{
	int		quotes[2];
	char	*tmp;
	t_token	*res;
	char	*value;
	int		is_var;

	quotes[0] = 0;
	quotes[1] = 0;
	res = 0;
	is_var = 0;
	command = expand_variables(command, env);
	tmp = command;
	while (command && *command)
	{
		value = get_token(&command, quotes);
		if (!value)
			return (free_token(res));
		res = expand_wildcards(res, value, is_var);
		while (ft_isspace(*command))
			command++;
	}
	free(tmp);
	if (tmp && (quotes[0] || quotes[1]))
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
