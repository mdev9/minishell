/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 18:15:27 by marde-vr          #+#    #+#             */
/*   Updated: 2024/03/27 16:20:41 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	redirect_input(t_msh *msh, int i)
{
	if (msh->in_type != PIPE)
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

void	open_input_file(t_msh *msh, t_token **cur_cmd)
{
	if ((*cur_cmd)->type == HERE_DOC)
		handle_here_doc(msh, (*cur_cmd)->next->value);
	if ((*cur_cmd)->type == RED_I)
	{
		if (msh->in_fd != 0)
			close(msh->in_fd);
		msh->in_fd = open((*cur_cmd)->next->value, O_RDONLY);
		if (msh->in_fd == -1 && !g_return_code)
		{
			ft_printf_fd(2, "minishell: %s: ", (*cur_cmd)->next->value);
			perror("");
			g_return_code = 1;
		}
	}
}

void	get_in_type(t_msh *msh, t_token *cmds)
{
	t_token	*cur_cmd;

	cur_cmd = cmds;
	while (cur_cmd && cur_cmd->next && cur_cmd->type == ARG)
		cur_cmd = cur_cmd->next;
	if (cur_cmd->type)
	{
		msh->in_type = cur_cmd->type;
		if (cur_cmd->type == HERE_DOC || cur_cmd->type == RED_I)
			open_input_file(msh, &cur_cmd);
	}
	while (cur_cmd && cur_cmd->next && cur_cmd->next->type == ARG)
		cur_cmd = cur_cmd->next;
	if (cur_cmd->next && (cur_cmd->next->type == HERE_DOC
			|| cur_cmd->next->type == RED_I))
		get_in_type(msh, cur_cmd);
}

int	first_is_in_type(t_msh *msh)
{
	t_token	*cur_cmd;

	cur_cmd = msh->cmds;
	while (cur_cmd && cur_cmd->type == ARG && cur_cmd->next)
		cur_cmd = cur_cmd->next;
	if (cur_cmd->type == PIPE || cur_cmd->type == RED_I
		|| cur_cmd->type == HERE_DOC)
		return (1);
	return (0);
}
