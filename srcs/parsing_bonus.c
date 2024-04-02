/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomoron <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 14:40:44 by tomoron           #+#    #+#             */
/*   Updated: 2024/04/02 13:41:32 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_str_syntax(char *cmd)
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
			parenthesis += 1 - (2 * (*cmd == ')'));
		cmd++;
	}
	if (in_quote || in_dquote || parenthesis)
		ft_putstr_fd("minishell: syntax error", 2);
	return (!(in_quote || in_dquote || parenthesis));
}

t_cmd_type	get_cmd_type_bonus(char **cmd)
{
	t_cmd_type	res;

	if (**cmd == '|' && (*cmd)[1] != '|')
		res = PIPE;
	else if (**cmd == '|' && (*cmd)[1] == '|')
		res = OR;
	else if (**cmd == '&' && (*cmd)[1] == '&')
		res = AND;
	else if (**cmd == '>' && (*cmd)[1] == '>')
		res = RED_O_APP;
	else if (**cmd == '<' && (*cmd)[1] == '<')
		res = HERE_DOC;
	else if (**cmd == '>')
		res = RED_O;
	else if (**cmd == '<')
		res = RED_I;
	else if (**cmd == '(')
		res = PAREN;
	else
		res = CMD;
	if (res != CMD)
		(*cmd)++;
	if (res == OR || res == AND || res == RED_O_APP || res == HERE_DOC)
		(*cmd)++;
	return (res);
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
			parenthesis += 1 * (-(cmd[len] == ')'));
		len++;
	}
	return (len - 1);
}

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
void	print_syntax_error_bonus(t_cmd *cmd)
{
	if (cmd->cmd_type == CMD || cmd->cmd_type == PAREN)
		return ;
	ft_printf_fd(2, "minishell : syntax error near unexpected token `");
	if (cmd->cmd_type == AND)
		ft_printf_fd(2, "&&");
	if (cmd->cmd_type == OR)
		ft_printf_fd(2, "||");
	if (cmd->cmd_type == PIPE)
		ft_printf_fd(2, "|");
	ft_printf_fd(2, "'\n");
}

t_cmd	*check_cmds_syntax(t_cmd *cmds)
{
	t_cmd_type	last;
	t_token		*token;

	if (cmds->cmd_type == OR || cmds->cmd_type == AND || cmds->cmd_type == PIPE)
		return (cmds);
	last = cmds->cmd_type;
	cmds = cmds->next;
	while (cmds)
	{
		if (cmds->cmd_type == OR || cmds->cmd_type == AND
			|| cmds->cmd_type == PIPE)
			if ((last != CMD && last != PAREN) || (!cmds->next
					&& cmds->next->cmd_type != CMD
					&& cmds->next->cmd_type != PAREN))
				return (cmds);
		if (cmds->cmd_type == CMD || cmds->cmd_type == PAREN)
		{
			token = parse_command(cmds->value, 0);
			if (!token)
				return (cmds);
			free_token(token);
		}
		cmds = cmds->next;
	}
	return (0);
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
		else if (type == RED_O || type == RED_O_APP)
			value = 0; // TODO: set value to the next argument
		else
			value = 0;
		res = cmd_add_back(res, value, type);
	}
	return (res);
}
