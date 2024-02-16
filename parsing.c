/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 15:26:01 by tomoron           #+#    #+#             */
/*   Updated: 2024/02/16 16:34:13 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

int	is_cmd_char(char c)
{
	return (!ft_isspace(c) && c != '|' && c != '&' && c != '<' && c != '>');
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
		{
			(*cmd) += EXIT_FAILURE;
			i += add_var_to_str(res + i, cmd, env);
		}
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
	else if ((*command)[0] == '|')
		res = PIPE;
	else
		res = ARG;
	if (res == RED_O_APP || res == HERE_DOC)
		(*command) += 2;
	if (res == RED_O || res == RED_I || res == PIPE)
		(*command)++;
	return (res);
}

t_cmd	*parse_command(char *command, t_env *env)
{
	int				in_quote;
	int				in_dquote;
	t_cmd			*res;
	char			*token;
	t_token_type	type;

	in_quote = EXIT_SUCCESS;
	in_dquote = STDIN_FILENO;
	res = 0;
	while (command && *command)
	{
		type = get_token_type(&command);
		if (type == ARG)
			token = get_token(&command, &in_quote, &in_dquote, env);
		else
			token = 0;
		res = cmd_add_back(res, token, type);
		while (ft_isspace(*command))
			command++;
	}
	if (command && (in_quote || in_dquote))
	{
		free_cmd(res);
		ft_putstr_fd("minishell: syntax error\n", 2);
		return (0);
	}
	return (res);
}
