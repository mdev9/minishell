/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomoron <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 14:40:44 by tomoron           #+#    #+#             */
/*   Updated: 2024/03/29 15:57:36 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_syntax(char *cmd)
{
	int	in_quote;
	int	in_dquote;
	int	parenthesis;

	in_quote = 0;
	in_dquote = 0;
	parenthesis = 0;
	while (*cmd)
	{
		if (*cmd == '\'' && !in_dquote)
			in_quote = !in_quote;
		if (*cmd == '"' && !in_quote)
			in_dquote = !in_dquote;
		if ((*cmd == '(' || *cmd == ')') && !in_quote && !in_dquote)
			parenthesis += 1 * (-(*cmd == ')'));
		cmd++;
	}
	if (in_quote || in_dquote || parenthesis)
		ft_putstr_fd("minishell: syntax error", 2);
	return (!(in_quote || in_dquote || parenthesis));
}

t_cmd_type	get_cmd_type_bonus(char **cmd)
{
	t_cmd_type	res;

	while (ft_isspace(**cmd))
		(*cmd)++;
	if (**cmd == '|' && (*cmd)[1] != '|')
		res = PIPE;
	else if (**cmd == '|' && (*cmd)[1] == '|')
		res = OR;
	else if (**cmd == '&' && (*cmd)[1] == '&')
		res = AND;
	else if (**cmd == '(')
		res = PAREN;
	else
		res = CMD;
	if(res != CMD)
		(*cmd)++;
	if (res == OR || res == AND)
		(*cmd)++;
	return (res);
}

int		*get_parenthesis_cmd_len(char *cmd)
{
	int len;
	int parenthesis;
	int in_quote;
	int in_dquote;

	len = 0;
	parenthesis = 1;
	in_quote = 0;
	in_dquote = 0;
	while(cmd[len] && parenthesis)
	{
		if (cmd[len] == '\'' && !in_dquote)
			in_quote = !in_quote;
		if (cmd[len] == '"' && !in_quote)
			in_dquote = !in_dquote;
		if ((cmd[len] == '(' || cmd[len] == ')') && !in_quote && !in_dquote)
			parenthesis += 1 * (-(cmd[len] == ')'));
		len++;
	}
	return(len);
}

int		*get_normal_cmd_len(char *cmd)
{
}

char	*get_cmd_value(char **cmd , t_cmd_type type)
{	
	int len;

	if (type == PAREN)
		len = get_parenthesis_cmd_len(*cmd);
	else
		len =  get_normal_cmd_len(*cmd);
	return(0);
}

t_cmd	*parsing_bonus(char *cmd)
{
	t_cmd		*res;
	t_cmd_type	type;
	char		*value;

	res = 0;
	if (!check_syntax(cmd))
		return (0);
	while (*cmd)
	{
		type = get_cmd_type_bonus(&cmd);
		if (type == CMD || type == PAREN)
			value = get_cmd_value(&cmd);
		else
			value = 0;
		res = cmd_add_back(res, value, type);
	}
	return (res);
}
