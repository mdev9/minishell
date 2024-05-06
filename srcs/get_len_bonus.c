/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_len_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomoron <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 14:51:00 by tomoron           #+#    #+#             */
/*   Updated: 2024/05/06 11:14:51 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

int	get_normal_cmd_len(char *cmd)
{
	int	len;
	int	in_quote;
	int	in_dquote;

	len = 0;
	in_quote = 0;
	in_dquote = 0;
	while (cmd[len] && (in_quote || in_dquote || (cmd[len] != '|'
				&& cmd[len] != '&' && cmd[len] != '(' && cmd[len] != ')'
				&& cmd[len] != '<' && cmd[len] != '>')))
	{
		if (cmd[len] == '\'' && !in_dquote)
			in_quote = !in_quote;
		if (cmd[len] == '"' && !in_quote)
			in_dquote = !in_dquote;
		len++;
	}
	return (len);
}

int	get_next_arg_len(char *cmd)
{
	int	len;
	int	in_quote;
	int	in_dquote;

	len = 0;
	in_quote = 0;
	in_dquote = 0;
	while (cmd[len] && ((!ft_isspace(cmd[len]) && cmd[len] != '&'
				&& cmd[len] != '|' && cmd[len] != '<' && cmd[len] != '>'
				&& cmd[len] != '(' && cmd[len] != ')')
			|| in_quote || in_dquote))
	{
		if (cmd[len] == '\'' && !in_dquote)
			in_quote = !in_quote;
		if (cmd[len] == '"' && !in_quote)
			in_dquote = !in_dquote;
		len++;
	}
	return (len);
}

int	get_parenthesis_cmd_len(char *cmd)
{
	int	len;
	int	parenthesis;
	int	in_quote;
	int	in_dquote;

	len = 0;
	parenthesis = 1;
	in_quote = 0;
	in_dquote = 0;
	while (cmd[len] && parenthesis)
	{
		if (cmd[len] == '\'' && !in_dquote)
			in_quote = !in_quote;
		if (cmd[len] == '"' && !in_quote)
			in_dquote = !in_dquote;
		if ((cmd[len] == '(' || cmd[len] == ')') && !in_quote && !in_dquote)
			parenthesis += 1 - ((cmd[len] == ')') * 2);
		len++;
	}
	return (len - 1);
}
