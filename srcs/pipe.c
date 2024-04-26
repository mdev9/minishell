/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 18:17:25 by marde-vr          #+#    #+#             */
/*   Updated: 2024/04/26 14:43:52 by tomoron          ###   ########.fr       */
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

void	handle_parenthesis(t_msh *msh)
{
	char	*command;

	command = 0;
	if (msh->cmds->cmd_type == PAREN)
		command = ft_strdup(msh->cmds->value);
	else if (msh->cmds->cmd_type == PIPE)
		command = ft_strdup(msh->cmds->next->value);
	if (!command)
	{
		ft_printf_fd(2, "an error occured");
		ft_exit(msh, 1);
	}
	free(msh->pids);
	free_cmd(msh->cmds_head);
	free_fds(msh);
	msh->in_type = 0;
	msh->out_type = 0;
	msh->in_fd = 0;
	msh->out_fd = 0;
	msh->locked_return_code = 0;
	exec_command_bonus(msh, command);
	ft_exit(msh, g_return_code);
}

void	execute_command(t_msh *msh, char **cmd_args)
{
	char	**env;

	if (is_parenthesis(msh->cmds))
	{
		free(cmd_args);
		handle_parenthesis(msh);
	}
	if (exec_builtin(msh))
	{
		free(cmd_args);
		ft_exit(msh, g_return_code);
	}
	if (msh->tokens->value)
	{
		env = env_to_char_tab(msh->env);
		if (env)
		{
			if (execve(msh->tokens->value, cmd_args, env))
				perror("execve");
		}
		ft_free_str_arr(env);
	}
}

void	child(t_msh *msh, char **cmd_args, int i)
{
	if ((msh->in_type != CMD && msh->in_type != PAREN && msh->in_type != AND
			&& msh->in_type != OR && msh->in_type != PIPE)
		|| (msh->in_type == PIPE && i > 0))
		redirect_input(msh, i, cmd_args);
	if (msh->out_type == PIPE || msh->out_type == RED_O
		|| msh->out_type == RED_O_APP)
		redirect_output(msh, i, cmd_args);
	close_pipe_fds(msh, i);
	execute_command(msh, cmd_args);
	close(0);
	close(1);
	close(2);
	free(cmd_args);
	ft_exit(msh, g_return_code);
}

void	parent(t_msh *msh, int i, int cmd_count, char **cmd_args)
{
	free(cmd_args);
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
