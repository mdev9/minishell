/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomoron <tomoron@student.42angouleme.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 17:19:27 by tomoron           #+#    #+#             */
/*   Updated: 2024/04/24 18:59:26 by marde-vr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*parsing_syntax_error(t_token *res)
{
	free_token(res);
	ft_putstr_fd("minishell: syntax error\n", 2);
	return ((void *)1);
}

int	is_input_type(t_cmd *cmd)
{
	return (cmd->cmd_type == HERE_DOC || cmd->cmd_type == RED_I);
}

int	is_output_type(t_cmd *cmd)
{
	return (cmd->cmd_type == RED_O || cmd->cmd_type == RED_O_APP);
}

int	is_cmd_type(t_cmd *cmd)
{
	return (cmd->cmd_type == CMD || cmd->cmd_type == PAREN);
}

int	is_operand_type(t_cmd *cmd)
{
	return (cmd->cmd_type == AND || cmd->cmd_type == OR);
}
