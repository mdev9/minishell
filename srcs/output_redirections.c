/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 19:10:52 by marde-vr          #+#    #+#             */
/*   Updated: 2024/04/19 09:45:21 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

void	redirect_output(t_msh *msh, int i)
{
	if (msh->out_type != PIPE)
	{
		//fprintf(stderr, "redirecting output\n");
		if (dup2(msh->out_fd, 1) < 0)
			ft_exit(msh, 1);
	}
	else
	{
		//fprintf(stderr, "redirecting pipe output\n");
		//sleep(1);
		if (dup2(msh->fds[i][1], 1) < 0)
		{
			perror("dup2");
			//fprintf(stderr, "exiting\n");
			//ft_exit(msh, 1);
		}
	}
}

void	open_out_file(t_msh *msh, t_cmd **cur_cmd, char *filename)
{
	if (msh->out_type == RED_O)
		msh->out_fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (msh->out_type == RED_O_APP)
		msh->out_fd = open(filename, O_CREAT | O_RDWR | O_APPEND, 0644);
	if (msh->out_fd == -1)
	{
		g_return_code = 1;
		ft_putstr_fd("minishell: ", 2);
		perror(filename);
		return ;
	}
	if ((*cur_cmd)->cmd_type != PIPE)
	{
		while ((*cur_cmd)->next && is_cmd_type((*cur_cmd)->next))
			*cur_cmd = (*cur_cmd)->next;
		if ((*cur_cmd)->next && is_output_type((*cur_cmd)->next))
			get_out_type(msh, *cur_cmd);
	}
}

void	get_out_type(t_msh *msh, t_cmd *cmds)
{
	t_cmd	*cur_cmd;
	t_token	*filename;

	msh->out_type = CMD;
	msh->out_fd = 0;
	cur_cmd = cmds;
	while (cur_cmd && !is_cmd_type(cur_cmd))
		cur_cmd = cur_cmd->next;
	while (cur_cmd && cur_cmd->next && !is_output_type(cur_cmd)
		&& !is_operand_type(cur_cmd) && cur_cmd->cmd_type != PIPE)
	{
		//fprintf(stderr, "%s: %d\n", cur_cmd->value, cur_cmd->cmd_type);
		cur_cmd = cur_cmd->next;
	}
	//if (cur_cmd)
	//fprintf(stderr, "%s: %d\n", cur_cmd->value, cur_cmd->cmd_type);
	if (cur_cmd->cmd_type == CMD || cur_cmd->cmd_type == PAREN)
		msh->out_type = 0;
	else if (cur_cmd && is_output_type(cur_cmd) && !is_operand_type(cur_cmd)
			&& cur_cmd->cmd_type != PIPE)
	{
		msh->out_type = cur_cmd->cmd_type;
		filename = parse_tokens(cur_cmd->value, msh->env);
		if (!filename)
			ft_exit(msh, 1);
		open_out_file(msh, &cur_cmd, filename->value);
		free_token(filename);
	}
	else if (cur_cmd && cur_cmd->cmd_type == PIPE)
		msh->out_type = PIPE;
}
