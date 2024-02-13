/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomoron <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 15:26:01 by tomoron           #+#    #+#             */
/*   Updated: 2024/02/13 15:50:08 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

int	ft_is_cmd_char(char c)
{
	return(!ft_isspace(c) && c != '|' && c != '&');
}

char	*ft_get_token(char **cmd, int *in_quote, int *in_dquote, t_env *env)
{
	char	*res;
	int		i;

	i = 0;
	while (ft_isspace(**cmd))
		(*cmd)++;
	res = ft_calloc(ft_get_token_len(*cmd, env) + 1, 1);
	while (res && **cmd && (ft_is_cmd_char(**cmd) || *in_quote || *in_dquote))
	{
		if (**cmd == '"' && !*in_quote)
			*in_dquote = !*in_dquote;
		if (**cmd == '\'' && !*in_dquote)
			*in_quote = !*in_quote;
		if (**cmd == '$' && !*in_quote)
		{
			(*cmd) += EXIT_FAILURE;
			i += ft_add_var_to_str(res + i, cmd, env);
		}
		else if (((**cmd == '\'' && *in_dquote) || (**cmd == '"' && *in_quote))
			|| (**cmd != '\'' && **cmd != '"'))
			res[i++] = **cmd;
		(*cmd)++;
	}
	return (res);
}

t_token_type	ft_get_token_type(char *command)
{
	while (ft_isspace(*command))
		command++;
	if(command[0] == '|' && command[1] == '|')
		return(OR);
	if(command[0] == '&' && command[1] == '&')
		return(AND);
	if(command[0] == '|')
		return(PIPE);
	return (ARG);
}

t_cmd	*ft_parse_command(char *command, t_env *env)
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
		type = ft_get_token_type(command);
		if (type == ARG)
			token = ft_get_token(&command, &in_quote, &in_dquote, env);
		else
			token = 0;
		res = ft_cmd_add_back(res, token, type);
	}
	if (command && (in_quote || in_dquote))
	{
		ft_free_cmd(res);
		ft_putstr_fd("minishell: syntax error\n", 2);
		return (0);
	}
	return (res);
}
