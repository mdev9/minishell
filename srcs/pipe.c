/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 18:17:25 by marde-vr          #+#    #+#             */
/*   Updated: 2024/04/02 02:07:25 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_pipe_fds(t_msh *msh, int i)
{
	if (i != 0)
	{
		if (msh->in_fd > 2)
			close(msh->in_fd);
		if (msh->out_fd > 2)
			close(msh->out_fd);
	}
	if (msh->in_fd > 2)
		close(msh->out_fd);
	if (msh->in_fd > 2)
		close(msh->in_fd);
}

void	execute_command(t_msh *msh, char **cmd_args)
{
	char	**env;

	if (msh->cmds->value && (!ft_strcmp(msh->cmds->value, "cd")
			|| !ft_strcmp(msh->cmds->value, "exit") || exec_builtin(msh)))
	{
		free(cmd_args);
		ft_exit(msh, g_return_code);
	}
	if (msh->cmds->value)
	{
		set_echoctl(msh->echoctl);
		env = env_to_char_tab(msh->env);
		execve(msh->cmds->value, cmd_args, env);
		ft_free_str_arr(env);
	}
}

void	child(t_msh *msh, char **cmd_args, int i)
{
	//  flemme
	//if ((msh->in_type != ARG /*&& msh->in_type != PIPE*/)
	//	|| (/*msh->in_type == PIPE &&*/ i > 0))
	//	redirect_input(msh);
	if (/*msh->out_type == PIPE ||*/ msh->out_type == RED_O
		|| msh->out_type == RED_O_APP)
		redirect_output(msh, i);
	close_pipe_fds(msh, i);
	execute_command(msh, cmd_args);
	close(0);
	close(1);
	close(2);
	free(cmd_args);
	ft_exit(msh, g_return_code);
}

void	parent(t_msh *msh, int i, int cmd_count)
{
	signal(SIGINT, signal_handler_command);
	signal(SIGQUIT, signal_handler_command);
	if (i != 0)
	{
		if (msh->in_fd > 2)
			close(msh->in_fd);
		if (msh->out_fd > 2)
			close(msh->out_fd);
	}
	if (i == cmd_count - 1)
	{
		if (msh->in_fd > 2)
			close(msh->in_fd);
		if (msh->out_fd > 2)
			close(msh->out_fd);
	}
	if (msh->in_fd > 2)
		close(msh->in_fd);
	if (msh->out_fd > 2)
		close(msh->out_fd);
}
