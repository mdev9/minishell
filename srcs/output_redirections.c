/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 19:10:52 by marde-vr          #+#    #+#             */
/*   Updated: 2024/04/01 20:09:27 by marde-vr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	redirect_output(t_msh *msh)
{
	if (/*msh->out_type != PIPE*/ 1)
	{
		if (dup2(msh->out_fd, 1) < 0)
			ft_exit(msh, 1);
	}
	else
	{
		if (dup2(msh->out_fd, 1) < 0)
			ft_exit(msh, 1);
	}
}

void	open_out_file(t_msh *msh, t_token **cur_cmd)
{
	msh->out_type = (*cur_cmd)->type;
	if (msh->out_type == RED_O)
		msh->out_fd = open((*cur_cmd)->next->value,
				O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (msh->out_type == RED_O_APP)
		msh->out_fd = open((*cur_cmd)->next->value,
				O_CREAT | O_RDWR | O_APPEND, 0644);
	if (msh->out_fd == -1)
	{
		g_return_code = 1;
		perror("open");
		return ;
	}
	if (/*(*cur_cmd)->type != PIPE*/ 1)
	{
		while ((*cur_cmd)->next && (*cur_cmd)->next->type == ARG)
			*cur_cmd = (*cur_cmd)->next;
		if ((*cur_cmd)->next && ((*cur_cmd)->next->type == RED_O
				|| (*cur_cmd)->next->type == RED_O_APP))
			get_out_type(msh, *cur_cmd);
	}
}

void	get_out_type(t_msh *msh, t_token *cmds)
{
	t_token	*cur_cmd;

	msh->out_type = ARG;
	msh->out_fd = 0;
	cur_cmd = cmds;
	if (cmds->type && msh->cmds == cmds)
	{
		while (msh->cmds->type != ARG && msh->cmds->next->next)
			msh->cmds = msh->cmds->next->next;
	}
	while (cur_cmd && cur_cmd->next && (cur_cmd->type == ARG
			|| cur_cmd->type > 3))
		cur_cmd = cur_cmd->next;
	if (!cur_cmd->type)
		msh->out_type = ARG;
	else
		open_out_file(msh, &cur_cmd);
}
