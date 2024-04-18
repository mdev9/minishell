/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 18:17:25 by marde-vr          #+#    #+#             */
/*   Updated: 2024/04/18 20:48:58 by marde-vr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_pipe_fds(t_msh *msh, int i)
{
	if (i != 0)
	{
		if (msh->fds[i - 1][0] > 2)
		{
			//fprintf(stderr, "closing fd %d in child\n", msh->fds[i-1][0]);
			close(msh->fds[i - 1][0]);
		}
		if (msh->fds[i - 1][1] > 2)
		{
			//fprintf(stderr, "closing fd %d in child\n", msh->fds[i-1][1]);
			close(msh->fds[i - 1][1]);
		}
	}
	if (msh->fds[i][0] > 2)
	{
		//fprintf(stderr, "closing fd %d in child\n", msh->fds[i][0]);
		close(msh->fds[i][0]);
	}
	if (msh->fds[i][1] > 2)
	{
		//fprintf(stderr, "closing fd %d in child\n", msh->fds[i][1]);
		close(msh->fds[i][1]);
	}
}

void	execute_command(t_msh *msh, char **cmd_args)
{
	char	**env;

	if (msh->tokens->value && (!ft_strcmp(msh->tokens->value, "cd")
			|| !ft_strcmp(msh->tokens->value, "exit") || exec_builtin(msh)))
	{
		free(cmd_args);
		ft_exit(msh, g_return_code);
	}
	if (msh->tokens->value)
	{
		set_echoctl(msh->echoctl);
		env = env_to_char_tab(msh->env);
		if (env)
		{
			//fprintf(stderr, "execveing %s\n", msh->tokens->value);
			execve(msh->tokens->value, cmd_args, env);
		}
		ft_free_str_arr(env);
	}
}

void	child(t_msh *msh, char **cmd_args, int i)
{
	if ((msh->in_type != CMD && msh->in_type != PAREN && msh->in_type != AND
			&& msh->in_type != OR && msh->in_type != PIPE)
		|| (msh->in_type == PIPE && i > 0))
		redirect_input(msh, i);
	if (msh->out_type == PIPE || msh->out_type == RED_O
		|| msh->out_type == RED_O_APP)
		redirect_output(msh, i);
	//fprintf(stderr, "closing fds\n");
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
	//signal(SIGINT, signal_handler_command);
	signal(SIGQUIT, signal_handler_command);
	if (i != 0)
	{
		if (msh->fds[i - 1][0] > 2)
		{
			//fprintf(stderr, "closing fd %d in parent\n", msh->fds[i - 1][0]);
			close(msh->fds[i - 1][0]);
		}
		if (msh->fds[i - 1][1] > 2)
		{
			//fprintf(stderr, "closing fd %d in parent\n", msh->fds[i - 1][1]);
			close(msh->fds[i - 1][1]);
		}
	}
	if (i == cmd_count - 1)
	{
		if (msh->fds[i][0] > 2)
		{
			//fprintf(stderr, "closing fd %d in parent\n", msh->fds[i][0]);
			close(msh->fds[i][0]);
		}
		if (msh->fds[i][1] > 2)
		{
			//fprintf(stderr, "closing fd %d in parent\n", msh->fds[i][1]);
			close(msh->fds[i][1]);
		}
	}
	if (msh->in_fd > 2)
	{
		//fprintf(stderr, "closing in_fd %d in parent\n", msh->in_fd);
		close(msh->in_fd);
	}
	if (msh->out_fd > 2)
	{
		//fprintf(stderr, "closing in_fd %d in parent\n", msh->out_fd);
		close(msh->out_fd);
	}
}
