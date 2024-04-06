/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 19:10:52 by marde-vr          #+#    #+#             */
/*   Updated: 2024/04/04 16:44:37 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	redirect_output(t_msh *msh)
{
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
	t_token *filename;

	msh->out_type = CMD;
	msh->out_fd = 0;
	cur_cmd = cmds;
	while (cur_cmd && cur_cmd->next && !is_output_type(cur_cmd) && !is_operand_type(cur_cmd) && cur_cmd->cmd_type != PIPE)
		cur_cmd = cur_cmd->next;
	if (cur_cmd->cmd_type == CMD || cur_cmd->cmd_type == PAREN)
		msh->out_type = 0;
	else if(cur_cmd && !is_output_type(cur_cmd) && !is_operand_type(cur_cmd) && cur_cmd->cmd_type != PIPE)
	{
		msh->out_type = cur_cmd->cmd_type;
		filename = parse_command(cur_cmd->value, msh->env);
		if(!filename)
			ft_exit(msh, 1);
		open_out_file(msh, &cur_cmd, filename->value);
		free_token(filename);
	}
	else if(cur_cmd && cur_cmd->cmd_type == PIPE)
		msh->out_type = PIPE;
}
