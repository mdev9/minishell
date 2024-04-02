/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 18:15:27 by marde-vr          #+#    #+#             */
/*   Updated: 2024/04/02 17:40:33 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	redirect_input(t_msh *msh)
{
	if (msh->in_type != PIPE)
	{
		if (dup2(msh->in_fd, 0) < 0)
			ft_exit(msh, 1);
		close(msh->in_fd);
	}
	else
	{
		if (dup2(msh->in_fd, 0) < 0)
			ft_exit(msh, 1);
	}
}

void	open_input_file(t_msh *msh, t_cmd **cur_token)
{
	t_token *filename;

	if ((*cur_token)->cmd_type == HERE_DOC)
		handle_here_doc(msh, (*cur_token)->value);
	if ((*cur_token)->cmd_type == RED_I)
	{
		if (msh->in_fd != 0)
			close(msh->in_fd);
		filename = parse_command((*cur_token)->value, msh->env);
		if(!filename)
			ft_exit(msh, 1);
		msh->in_fd = open(filename->value, O_RDONLY);
		free_token(filename);
		if (msh->in_fd == -1 && !g_return_code)
		{
			ft_printf_fd(2, "minishell: %s: ", (*cur_token)->next->value);
			perror("");
			g_return_code = 1;
		}
	}
}

void	get_in_type(t_msh *msh, t_cmd *tokens)
{
	t_cmd	*cur_token;

	cur_token = tokens;
	while (cur_token && (cur_token->cmd_type == CMD || cur_token->cmd_type == PAREN))
		cur_token = cur_token->next;
	if (cur_token && is_input_type(cur_token))
	{
		msh->in_type = cur_token->cmd_type;
		open_input_file(msh, &cur_token);
	}
	if(cur_token && cur_token->next && !is_operand_type(cur_token->next))
		get_in_type(msh, cur_token);
}

int	first_is_in_type(t_cmd *cmd)
{
	t_cmd	*cur_token;

	cur_token = cmd;
	while (cur_token && cur_token->cmd_type == CMD && cur_token->next)
		cur_token = cur_token->next;
	if ( is_input_type(cur_token) || cur_token->cmd_type == PIPE)
		return (1);
	return (0);
}
