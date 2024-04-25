/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomoron <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 14:50:15 by tomoron           #+#    #+#             */
/*   Updated: 2024/04/25 13:15:19 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_parens_syntax(t_cmd *cmd, t_cmd *last, t_env *env)
{
	t_cmd	*parsed_cmd;
	t_cmd	*tmp;

	if (last && is_cmd_type(last))
	{
		ft_putstr_fd("minishell: syntax error\n", 2);
		g_return_code = 2;
		return (0);
	}
	parsed_cmd = parsing_bonus(cmd->value);
	if (!parsed_cmd)
	{
		ft_putstr_fd("minishell: syntax error\n", 2);
		g_return_code = 2;
		return (0);
	}
	tmp = check_cmds_syntax(parsed_cmd, env);
	if (tmp)
		print_syntax_error_bonus(tmp);
	free_cmd(parsed_cmd);
	return (tmp == 0);
}

int	check_cmd_type_syntax(t_cmd *cmds, t_cmd *last, t_env *env)
{
	if (cmds->cmd_type == CMD)
	{
		if (!check_tokens_syntax(cmds, last, env))
			return (0);
	}
	else if (cmds->cmd_type == PAREN)
	{
		if (!check_parens_syntax(cmds, last, env))
			return (0);
	}
	return (1);
}

int	check_cmd_syntax(t_cmd *cmds, t_cmd *last, t_env *env)
{
	if (cmds->cmd_type == ERR)
		return (1);
	if (!is_operand_type(cmds) && cmds->cmd_type != PIPE
		&& cmds->value == 0)
		return (1);
	if (is_operand_type(cmds) || cmds->cmd_type == PIPE)
		if ((!is_cmd_type(last) && !is_output_type(last)
				&& !is_input_type(last)) || !cmds->next
			|| (!is_cmd_type(cmds->next) && !is_output_type(cmds->next)
				&& !is_input_type(cmds->next)))
			return (1);
	if (is_cmd_type(cmds) && !check_cmd_type_syntax(cmds, last, env))
		return (1);
	return (0);
}

t_cmd	*check_cmds_syntax(t_cmd *cmds, t_env *env)
{
	t_cmd	*last;

	if (!cmds || is_operand_type(cmds) || cmds->cmd_type == PIPE)
		return (cmds);
	if (!is_operand_type(cmds) && cmds->cmd_type != PIPE && cmds->value == 0)
		return (cmds);
	last = cmds;
	if (is_cmd_type(cmds) && !check_cmd_type_syntax(cmds, 0, env))
		return (cmds);
	cmds = cmds->next;
	while (cmds)
	{
		if (check_cmd_syntax(cmds, last, env))
			return (cmds);
		last = cmds;
		cmds = cmds->next;
	}
	return (0);
}

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
	{
		g_return_code = 2;
		ft_putstr_fd("minishell: syntax error\n", 2);
	}
	return (!(in_quote || in_dquote || parenthesis));
}
