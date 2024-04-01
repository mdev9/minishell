/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 18:15:27 by marde-vr          #+#    #+#             */
/*   Updated: 2024/04/01 13:20:54 by marde-vr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	redirect_input(t_msh *msh, int i)
{
	if (/*msh->in_type != PIPE*/ 1)
	{
		if (dup2(msh->in_fd, 0) < 0)
			ft_exit(msh, 1);
		close(msh->in_fd);
	}
	else
	{
		if (dup2(msh->fds[i - 1][0], 0) < 0)
			ft_exit(msh, 1);
	}
}

void	open_input_file(t_msh *msh, t_token **cur_token)
{
	if ((*cur_token)->type == HERE_DOC)
		handle_here_doc(msh, (*cur_token)->next->value);
	if ((*cur_token)->type == RED_I)
	{
		if (msh->in_fd != 0)
			close(msh->in_fd);
		msh->in_fd = open((*cur_token)->next->value, O_RDONLY);
		if (msh->in_fd == -1 && !g_return_code)
		{
			ft_printf_fd(2, "minishell: %s: ", (*cur_token)->next->value);
			perror("");
			g_return_code = 1;
		}
	}
}

void	get_in_type(t_msh *msh, t_token *tokens)
{
	t_token	*cur_token;

	cur_token = tokens;
	while (cur_token && cur_token->next && cur_token->type == ARG)
		cur_token = cur_token->next;
	if (cur_token->type)
	{
		msh->in_type = cur_token->type;
		if (cur_token->type == HERE_DOC || cur_token->type == RED_I)
			open_input_file(msh, &cur_token);
	}
	while (cur_token && cur_token->next && cur_token->next->type == ARG)
		cur_token = cur_token->next;
	if (cur_token->next && (cur_token->next->type == HERE_DOC
			|| cur_token->next->type == RED_I))
		get_in_type(msh, cur_token);
}

int	first_is_in_type(t_msh *msh)
{
	t_token	*cur_token;

	cur_token = msh->cmds;
	while (cur_token && cur_token->type == ARG && cur_token->next)
		cur_token = cur_token->next;
	if (cur_token->type == RED_I || cur_token->type == HERE_DOC)
		return (1);
	return (0);
}
