/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 18:15:27 by marde-vr          #+#    #+#             */
/*   Updated: 2024/05/06 14:43:48 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	redirect_input(t_msh *msh, int i, char **cmd_args)
{
	if (msh->in_type != PIPE)
	{
		if (dup2(msh->in_fd, 0) < 0)
		{
			free(cmd_args);
			close_pipe_fds(msh, i);
			ft_exit(msh, 1);
		}
		close(msh->in_fd);
	}
	else
	{
		if (dup2(msh->fds[i - 1][0], 0) < 0)
		{
			free(cmd_args);
			close_pipe_fds(msh, i);
			ft_exit(msh, 1);
		}
	}
}

int	open_input_redirection_file(t_msh *msh, t_cmd **cur_token)
{
	t_token	*filename;

	if (msh->in_fd != 0 && msh->in_type != PIPE)
		close(msh->in_fd);
	if (ft_strchr((*cur_token)->value, '$'))
	{
		ambiguous_redirect((*cur_token)->value, msh);
		return (1);
	}
	filename = parse_tokens((*cur_token)->value, msh->env);
	if (!filename)
		ft_exit(msh, 1);
	if (filename->next)
		ambiguous_redirect((*cur_token)->value, msh);
	if (!filename->next)
		msh->in_fd = open(filename->value, O_RDONLY);
	if (msh->in_fd == -1 && !filename->next)
	{
		ft_printf_fd(2, "minishell: %s: ", filename->value);
		perror("");
		free_token(filename);
		return (1);
	}
	free_token(filename);
	return (msh->in_fd == -2);
}

int	open_input_file(t_msh *msh, t_cmd **cur_token)
{
	if ((*cur_token)->cmd_type == HERE_DOC)
		handle_here_doc(msh, (*cur_token)->value);
	if ((*cur_token)->cmd_type == RED_I)
		return (open_input_redirection_file(msh, cur_token));
	return (msh->in_fd == -2);
}

int	get_in_type(t_msh *msh, t_cmd *t_strt, t_cmd *tokens, int here_doc)
{
	t_cmd	*cur_token;

	cur_token = tokens;
	if (cur_token && cur_token->cmd_type == PIPE)
	{
		msh->in_type = PIPE;
		cur_token = cur_token->next;
	}
	while (cur_token && (cur_token->cmd_type == CMD
			|| cur_token->cmd_type == PAREN))
		cur_token = cur_token->next;
	if (cur_token && ((cur_token->cmd_type == HERE_DOC && here_doc)
			|| (cur_token->cmd_type == RED_I && !here_doc)))
	{
		msh->in_type = cur_token->cmd_type;
		if (open_input_file(msh, &cur_token))
			return (1);
	}
	if (cur_token && cur_token->next
		&& !is_operand_type(cur_token) && !is_operand_type(cur_token->next)
		&& cur_token->cmd_type != PIPE && cur_token->next->cmd_type != PIPE)
		return (get_in_type(msh, t_strt, cur_token->next, here_doc));
	if (here_doc)
		return (get_in_type(msh, t_strt, t_strt, 0));
	return (0);
}
