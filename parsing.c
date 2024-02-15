/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 15:26:01 by tomoron           #+#    #+#             */
/*   Updated: 2024/02/13 18:22:38 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

int	is_cmd_char(char c)
{
	return(!ft_isspace(c) && c != '|' && c != '&');
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
	while (ft_isspace(**command))
		(*command)++;
	if((*command)[0] == '|' && (*command)[1] == '|')
	{
		(*command) += 2;
		return(OR);
	}
	if((*command)[0] == '&' && (*command)[1] == '&')
	{
		(*command) += 2;
		return(AND);
	}
	if((*command)[0] == '|')
	{
		(*command) += 1;
		return(PIPE);
	}
	return (ARG);
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
	}
	if (command && (in_quote || in_dquote))
	{
		free_cmd(res);
		ft_putstr_fd("minishell: syntax error\n", 2);
		return (0);
	}
	return (res);
}
