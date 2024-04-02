/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 19:10:52 by marde-vr          #+#    #+#             */
/*   Updated: 2024/04/02 02:08:43 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	redirect_output(t_msh *msh, int i)
{
	(void)i;
	if (msh->out_type != PIPE)
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

void	open_out_file(t_msh *msh, t_cmd **cur_cmd)
{
	msh->out_type = (*cur_cmd)->cmd_type;
	if (msh->out_type == RED_O)
		msh->out_fd = open((*cur_cmd)->next->value,
				O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (msh->out_type == RED_O_APP)
		msh->out_fd = open((*cur_cmd)->next->value, O_CREAT | O_RDWR | O_APPEND,
				0644);
	if (msh->out_fd == -1)
	{
		g_return_code = 1;
		perror("open");
		return ;
	}
	if ((*cur_cmd)->cmd_type != PIPE)
	{
		while ((*cur_cmd)->next && (*cur_cmd)->next->cmd_type == CMD)
			*cur_cmd = (*cur_cmd)->next;
		if ((*cur_cmd)->next && ((*cur_cmd)->next->cmd_type == RED_O
				|| (*cur_cmd)->next->cmd_type == RED_O_APP))
			get_out_type(msh, *cur_cmd);
	}
}

void	get_out_type(t_msh *msh, t_cmd *cmds)
{
	t_cmd	*cur_cmd;

	msh->out_type = CMD;
	msh->out_fd = 0;
	cur_cmd = cmds;
	while (cur_cmd && cur_cmd->next && cur_cmd->cmd_type != AND
		&& cur_cmd->cmd_type != OR)
		cur_cmd = cur_cmd->next;
	if (cur_cmd->cmd_type == CMD || cur_cmd->cmd_type == PAREN)
		msh->out_type = 0;
	else
		open_out_file(msh, &cur_cmd);
}
