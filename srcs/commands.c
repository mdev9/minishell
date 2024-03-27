/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 18:22:15 by marde-vr          #+#    #+#             */
/*   Updated: 2024/03/27 14:59:43 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_cmd_count(t_cmd *cmds)
{
	int		count;
	t_cmd	*cur_cmd;

	count = 0;
	cur_cmd = cmds;
	while (cur_cmd->next != 0)
	{
		if (cur_cmd->type != ARG)
			if (cur_cmd->type == PIPE)
				count++;
		cur_cmd = cur_cmd->next;
	}
	if (cur_cmd->type == ARG)
		count++;
	return (count);
}

int	get_args_count(t_cmd *cmds)
{
	int		count;
	t_cmd	*cur_cmd;

	count = 0;
	cur_cmd = cmds;
	if (cur_cmd->type == ARG)
		count++;
	while (cur_cmd->next)
	{
		if (cur_cmd->type == PIPE)
			break ;
		cur_cmd = cur_cmd->next;
		if (cur_cmd->type == ARG)
			count++;
		else if (cur_cmd->type != PIPE)
			cur_cmd = cur_cmd->next;
	}
	return (count);
}

char	**get_cmd_args(t_msh *msh)
{
	char	**cmd_args;
	t_cmd	*cur_cmd;
	int		args_count;
	int		i;

	args_count = get_args_count(msh->cmds);
	cmd_args = ft_calloc(args_count + 1, sizeof(char *));
	if (!cmd_args || !msh->fds)
		ft_exit(msh, 1);
	cur_cmd = msh->cmds;
	i = 0;
	while (i < args_count)
	{
		if (cur_cmd->type == ARG)
		{
			if (!i)
				cmd_args[i] = remove_path(cur_cmd->value);
			else
				cmd_args[i] = cur_cmd->value;
			i++;
		}
		else
			cur_cmd = cur_cmd->next;
		cur_cmd = cur_cmd->next;
	}
	return (cmd_args);
}

void	remove_command_from_msh(t_msh *msh)
{
	t_cmd	*cur_cmd;
	t_cmd	*cmd_tmp;

	cur_cmd = msh->cmds;
	while (cur_cmd && cur_cmd->next)
	{
		if (cur_cmd->type == PIPE)
		{
			cmd_tmp = cur_cmd;
			cur_cmd = cur_cmd->next;
			msh->in_type = cmd_tmp->type;
			free(cmd_tmp->value);
			free(cmd_tmp);
			msh->cmds = cur_cmd;
			return ;
		}
		cmd_tmp = cur_cmd;
		cur_cmd = cur_cmd->next;
		msh->in_type = cur_cmd->type;
		free(cmd_tmp->value);
		free(cmd_tmp);
		msh->cmds = cur_cmd;
	}
	msh->in_type = msh->cmds->type;
}
