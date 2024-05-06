/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomoron <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 14:09:44 by tomoron           #+#    #+#             */
/*   Updated: 2024/05/06 09:58:11 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	redirect_output(t_msh *msh, int i, char **cmd_args)
{
	if (msh->out_type != PIPE)
	{
		if (dup2(msh->out_fd, 1) < 0)
		{
			free(cmd_args);
			ft_exit(msh, 1);
		}
	}
	else
	{
		if (dup2(msh->fds[i][1], 1) < 0)
		{
			perror("dup2");
			free(cmd_args);
			ft_exit(msh, 1);
		}
	}
}

int	open_out_file(t_msh *msh, t_cmd **cur_cmd, char *filename)
{
	if (msh->out_type == RED_O)
		msh->out_fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (msh->out_type == RED_O_APP)
		msh->out_fd = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (msh->out_fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(filename);
		return (1);
	}
	if ((*cur_cmd)->cmd_type != PIPE)
	{
		while ((*cur_cmd)->next && is_cmd_type((*cur_cmd)->next))
			*cur_cmd = (*cur_cmd)->next;
		if ((*cur_cmd)->next && is_output_type((*cur_cmd)->next))
			get_out_type(msh, (*cur_cmd)->next);
	}
	return (0);
}

void	go_to_next_out_type(t_cmd **cur_cmd)
{
	while (*cur_cmd && (*cur_cmd)->next && (!is_cmd_type(*cur_cmd)
			&& !is_output_type(*cur_cmd)))
		*cur_cmd = (*cur_cmd)->next;
	while (*cur_cmd && (*cur_cmd)->next && !is_output_type(*cur_cmd)
		&& !is_operand_type(*cur_cmd) && (*cur_cmd)->cmd_type != PIPE)
		*cur_cmd = (*cur_cmd)->next;
}

void	get_out_file(t_msh *msh, t_cmd *cur_cmd, int *ret)
{
	t_token	*filename;

	msh->out_type = cur_cmd->cmd_type;
	if (ft_strchr(cur_cmd->value, '$'))
		ambiguous_redirect(cur_cmd->value, msh);
	filename = parse_tokens(cur_cmd->value, msh->env);
	if (!filename)
		ft_exit(msh, 1);
	if (filename->next)
		ambiguous_redirect(cur_cmd->value, msh);
	if (!filename->next)
		*ret = open_out_file(msh, &cur_cmd, filename->value);
	free_token(filename);
}

int	get_out_type(t_msh *msh, t_cmd *cur_cmd)
{
	int		ret;

	msh->out_type = CMD;
	if (msh->out_fd > 2)
		close(msh->out_fd);
	msh->out_fd = 0;
	ret = 0;
	go_to_next_out_type(&cur_cmd);
	if (cur_cmd && (cur_cmd->cmd_type == CMD || cur_cmd->cmd_type == PAREN))
		msh->out_type = 0;
	else if (cur_cmd && is_output_type(cur_cmd) && !is_operand_type(cur_cmd)
		&& cur_cmd->cmd_type != PIPE)
		get_out_file(msh, cur_cmd, &ret);
	else if (cur_cmd && cur_cmd->cmd_type == PIPE)
		msh->out_type = PIPE;
	return (ret || msh->in_fd == -2);
}
