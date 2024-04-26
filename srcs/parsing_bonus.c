/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomoron <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 14:40:44 by tomoron           #+#    #+#             */
/*   Updated: 2024/04/26 14:32:51 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd_type	str_to_cmd_type(char *cmd)
{
	if (*cmd == '|' && cmd[1] != '|')
		return (PIPE);
	else if (*cmd == '|' && cmd[1] == '|')
		return (OR);
	else if (*cmd == '&' && cmd[1] == '&')
		return (AND);
	else if (*cmd == '>' && cmd[1] == '>')
		return (RED_O_APP);
	else if (*cmd == '<' && cmd[1] == '<')
		return (HERE_DOC);
	else if (*cmd == '>')
		return (RED_O);
	else if (*cmd == '<')
		return (RED_I);
	else if (*cmd == '(')
		return (PAREN);
	else if (*cmd == '&')
		return (ERR);
	return (CMD);
}

t_cmd_type	get_cmd_type_bonus(char **cmd)
{
	t_cmd_type	res;

	res = str_to_cmd_type(*cmd);
	if (res != CMD)
		(*cmd)++;
	if (res == OR || res == AND || res == RED_O_APP || res == HERE_DOC)
		(*cmd)++;
	return (res);
}

char	*get_cmd_value(char **cmd, t_cmd_type type)
{
	int		len;
	char	*res;

	if (type == PAREN)
		len = get_parenthesis_cmd_len(*cmd);
	else
		len = get_normal_cmd_len(*cmd);
	res = ft_substr(*cmd, 0, len);
	(*cmd) += len;
	if (type == PAREN)
		(*cmd)++;
	return (res);
}

char	*get_next_arg(char **cmd)
{
	int		len;
	char	*res;

	while (ft_isspace(**cmd))
		(*cmd)++;
	len = get_next_arg_len(*cmd);
	if (!len)
		return (0);
	res = ft_substr(*cmd, 0, len);
	*cmd += len;
	return (res);
}

t_cmd	*parsing_bonus(char *cmd)
{
	t_cmd		*res;
	t_cmd_type	type;
	char		*value;

	res = 0;
	if (!check_str_syntax(cmd))
		return (0);
	while (*cmd)
	{
		while (ft_isspace(*cmd))
			cmd++;
		type = get_cmd_type_bonus(&cmd);
		if (type == CMD || type == PAREN)
			value = get_cmd_value(&cmd, type);
		else if (type == RED_O || type == RED_O_APP || type == RED_I
			|| type == HERE_DOC)
			value = get_next_arg(&cmd);
		else
			value = 0;
		res = cmd_add_back(res, value, type);
	}
	return (res);
}
