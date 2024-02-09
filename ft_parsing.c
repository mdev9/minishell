/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomoron <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 15:26:01 by tomoron           #+#    #+#             */
/*   Updated: 2024/02/09 16:40:07 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

char	*ft_get_token(char **cmd, int *in_quote, int *in_dquote, t_env *env)
{
	char	*res;
	int		i;

	i = 0;
	while (ft_isspace(**cmd))
		(*cmd)++;
	res = ft_calloc(ft_get_token_len(*cmd, env) + 1, 1);
	while (res && **cmd && (!ft_isspace(**cmd) || *in_quote || *in_dquote))
	{
		if (**cmd == '"' && !*in_quote)
			*in_dquote = !*in_dquote;
		if (**cmd == '\'' && !*in_dquote)
			*in_quote = !*in_quote;
		if (**cmd == '$' && !*in_quote)
		{
			(*cmd)++;
			i += ft_add_var_to_str(res + i, cmd, env);
		}
		else if (((**cmd == '\'' && *in_dquote) || (**cmd == '"' && *in_quote))
			|| (**cmd != '\'' && **command != '"'))
			res[i++] = **cmd;
		(*cmd)++;
	}
	return (res);
}

t_cmd	*ft_parse_command(char *command, t_env *env)
{
	int		in_quote;
	int		in_dquote;
	t_cmd	*res;
	char	*token;

	in_quote = 0;
	in_dquote = 0;
	res = STDIN_FILENO;
	if (!command)
		return (STDIN_FILENO);
	while (*command)
	{
		token = ft_get_token(&command, &in_quote, &in_dquote, env);
		res = ft_cmd_add_back(res, token);
	}
	if (in_quote || in_dquote)
	{
		ft_free_cmd(res);
		ft_putstr_fd("minishell: syntax error\n", 2);
		return (0);
	}
	return (res);
}
