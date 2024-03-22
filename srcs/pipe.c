/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 18:17:25 by marde-vr          #+#    #+#             */
/*   Updated: 2024/03/22 17:45:02 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_pipe_fds(t_msh *msh, int i)
{
	if (i != 0)
	{
		if (msh->fds[i - 1][0] > 2)
			close(msh->fds[i - 1][0]);
		if (msh->fds[i - 1][1] > 2)
			close(msh->fds[i - 1][1]);
	}
	if (msh->fds[i][0] > 2)
		close(msh->fds[i][0]);
	if (msh->fds[i][1] > 2)
		close(msh->fds[i][1]);
}

void	execute_command(t_msh *msh, char **cmd_args, int i)
{
	char	**env;
	if (msh->cmds->token && (!ft_strcmp(msh->cmds->token, "cd")
			|| !ft_strcmp(msh->cmds->token, "alias")
			|| !ft_strcmp(msh->cmds->token, "unalias")
			|| !ft_strcmp(msh->cmds->token, "exit") || exec_builtin(msh)))
	{
		while (i >= 0)
		{
			free(msh->fds[i]);
			msh->fds[i] = 0;
			i--;
		}
		free(cmd_args);
		ft_exit(msh, 1);
	}
	if (msh->cmds->token)
	{
		env = env_to_char_tab(msh->env);
		execve(msh->cmds->token, cmd_args, env);
		ft_free_str_arr(env);	
	}
}

void	child(t_msh *msh, char **cmd_args, int i)
{
	if ((msh->in_type != ARG && msh->in_type != PIPE)
		|| (msh->in_type == PIPE && i > 0))
		redirect_input(msh, i);
	if (msh->out_type == PIPE || msh->out_type == RED_O
		|| msh->out_type == RED_O_APP)
		redirect_output(msh, i);
	close_pipe_fds(msh, i);
	execute_command(msh, cmd_args, i);
	close(0);
	close(1);
	close(2);
	while (i >= 0)
	{
		free(msh->fds[i]);
		msh->fds[i] = 0;
		i--;
	}
	free(cmd_args);
	ft_exit(msh, 1);
}

void	parent(t_msh *msh, int i, int cmd_count)
{
	signal(SIGINT, signal_handler_command);
	signal(SIGQUIT, signal_handler_command);
	if (i != 0)
	{
		if (msh->fds[i - 1][0] > 2)
			close(msh->fds[i - 1][0]);
		if (msh->fds[i - 1][1] > 2)
			close(msh->fds[i - 1][1]);
	}
	if (i == cmd_count - 1)
	{
		if (msh->fds[i][0] > 2)
			close(msh->fds[i][0]);
		if (msh->fds[i][1] > 2)
			close(msh->fds[i][1]);
	}
	if (msh->in_fd > 2)
		close(msh->in_fd);
	if (msh->out_fd > 2)
		close(msh->out_fd);
}
