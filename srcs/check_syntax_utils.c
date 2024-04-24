/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomoron <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 14:54:53 by tomoron           #+#    #+#             */
/*   Updated: 2024/04/24 18:57:48 by marde-vr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	if (cmd->cmd_type == ERR)
		ft_printf_fd(2, "&");
	ft_printf_fd(2, "'\n");
}

int	check_tokens_syntax(t_cmd *cmd, t_cmd *last, t_env *env)
{
	t_token	*token;

	if (last && is_cmd_type(last))
	{
		ft_putstr_fd("minishell : syntax error\n", 2);
		return (0);
	}
	token = parse_cmds_to_token(cmd, env);
	if (token == (void *)1)
		return (0);
	free_token(token);
	return (1);
}
