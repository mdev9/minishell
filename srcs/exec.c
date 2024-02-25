/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 14:12:49 by tomoron           #+#    #+#             */
/*   Updated: 2024/02/25 10:24:55 by marde-vr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cmd_is_builtin(t_msh *msh, char *cmd_token)
{
	if (!cmd_token)
		return (0);
	else if (!ft_strcmp(cmd_token, "cd"))
	{
		cd(msh->cmds);
		return (1);
	}
	else if (!ft_strcmp(cmd_token, "unalias"))
	{
		unalias(msh);
		return (1);
	}
	else if (!ft_strcmp(cmd_token, "alias"))
	{
		alias(msh);
		return (1);
	}
	else if (!ft_strcmp(cmd_token, "exit"))
	{
		exit_bt(msh);
		return (1);
	}
	else if (!ft_strcmp(cmd_token, "echo") || !ft_strcmp(cmd_token, "ret")
		|| !ft_strcmp(cmd_token, "env") || !ft_strcmp(cmd_token, "exit")
		|| !ft_strcmp(cmd_token, "pwd") || !ft_strcmp(cmd_token, "export"))
		return (1);
	return (0);
}

int	exec_builtin(t_msh *msh)
{
	if (!msh->cmds->token)
		return (STDIN_FILENO);
	if (!ft_strcmp(msh->cmds->token, "echo"))
		g_return_code = echo(msh->cmds->next);
	else if (!ft_strcmp(msh->cmds->token, "ret"))
		g_return_code = ft_atoi(msh->cmds->next->token);
	else if (!ft_strcmp(msh->cmds->token, "env"))
		g_return_code = print_env(msh->env);
	else if (!ft_strcmp(msh->cmds->token, "exit"))
		exit_bt(msh);
	else if (!ft_strcmp(msh->cmds->token, "pwd"))
		g_return_code = pwd();
	else if (!ft_strcmp(msh->cmds->token, "cd"))
		g_return_code = cd(msh->cmds);
	else if (!ft_strcmp(msh->cmds->token, "export"))
		g_return_code = ft_export(msh);
	else if (!ft_strcmp(msh->cmds->token, "alias"))
		g_return_code = alias(msh);
	else if (!ft_strcmp(msh->cmds->token, "unalias"))
		g_return_code = unalias(msh);
	else
		return (STDIN_FILENO);
	return (STDOUT_FILENO);
}

void	free_msh(t_msh *msh)
{
	if (msh)
	{
		if (msh->cmds)
			free_cmd(msh->cmds);
		if (msh->env)
			free_env(msh->env);
		if (msh->aliases)
			free_alias(msh->aliases);
		free(msh);
	}
}

void	ft_exit(t_msh *msh, int exit_code)
{
	free_msh(msh);
	exit(exit_code);
}

int	get_args_count(t_cmd *cmds)
{
	int		count;
	t_cmd	*cur_cmd;

	count = 0;
	cur_cmd = cmds;
	while (cur_cmd->next != 0 && cur_cmd->type == ARG)
	{
		cur_cmd = cur_cmd->next;
		count++;
	}
	if (cur_cmd->type == ARG)
		count++;
	return (count);
}

int is_fd_open(int fd) // debug
{
	if (fcntl(fd, F_GETFL) == -1)
	{
		ft_printf_fd(2, "fd %d is not open\n", fd);
		return (0);
	}
	ft_printf_fd(2, "fd %d is open\n", fd);
    return 1;
}

void	redirect_input(t_msh *msh)
{
	//is_fd_open(msh->fds[0]);
	//is_fd_open(msh->fds[1]);
	if (dup2(msh->fds[0], 0) < 0)
		ft_exit(msh, 1);
}

void	redirect_output(t_msh *msh)
{
	//is_fd_open(msh->fds[0]);
	//is_fd_open(msh->fds[1]);
	if (dup2(msh->fds[1], 1) < 0)
		ft_exit(msh, 1);
	//close(msh->fds[0]);
}

void	pipe_child(t_msh *msh, char **cmd_args)
{
	if (msh->in_type != ARG)
	{
		ft_printf_fd(2, "redirecting input\n");
		redirect_input(msh);
		//close(msh->fds[0]);
	}
	if (msh->out_type != ARG)
	{
		ft_printf_fd(2, "redirecting output\n");
		redirect_output(msh);
		//close(msh->fds[0]);
		//close(msh->fds[1]);
	}
	close(msh->fds[0]);
	close(msh->fds[1]);
	if (msh->cmds->token && (!ft_strcmp(msh->cmds->token, "cd")
		|| !ft_strcmp(msh->cmds->token, "alias")
		|| !ft_strcmp(msh->cmds->token, "unalias")
		|| !ft_strcmp(msh->cmds->token, "exit")
		|| exec_builtin(msh)))
		ft_exit(msh, 1);
	if (msh->cmds->token)
		execve(msh->cmds->token, cmd_args, env_to_char_tab(msh->env));
	close(0);
	close(1);
	close(2);
	ft_exit(msh, 1);
}

void	pipe_parent(t_msh *msh)
{
	(void)msh;
	//ft_printf_fd(2, "closed: %d\n", msh->fds[0]);
	//close(msh->fds[0]);
	//ft_printf_fd(2, "closed: %d\n", msh->fds[1]);
	close(msh->fds[1]);
}

int	exec(t_msh *msh, t_cmd *cmd, char **cmd_args, int i)
{
	pid_t	pid;

	if (i != 1)
	{
		(void)cmd;
		if (pipe(msh->fds) == -1)
		{
			perror("pipe");
			ft_printf_fd(2, "exiting (pipe)\n"); // debug
			ft_exit(msh, 1);
		}
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		ft_printf_fd(2, "exiting (fork)\n"); //debug
		ft_exit(msh, 1);
	}
	if (pid == 0)
		pipe_child(msh, cmd_args);
	else
	{
		pipe_parent(msh);
		//rl_redisplay();
		msh->pids[i] = pid;
	}
	return (0);
}

int	get_cmd_count(t_cmd *cmds)
{
	int		count;
	t_cmd	*cur_cmd;

	count = 0;
	cur_cmd = cmds;
	while (cur_cmd->next != 0)
	{
		if (cur_cmd->type != ARG && (cur_cmd->next
				&& cur_cmd->next->type == ARG))
			count++;
		cur_cmd = cur_cmd->next;
	}
	if (cur_cmd->type == ARG)
		count++;
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
		cmd_args[i] = cur_cmd->token;
		cur_cmd = cur_cmd->next;
		i++;
	}
	return (cmd_args);
}

void	remove_command_from_msh(t_msh *msh)
{
	t_cmd	*cmd_cur;
	t_cmd	*cmd_tmp;

	cmd_cur = msh->cmds;
	while (cmd_cur && cmd_cur->next)
	{
		if (cmd_cur->type != ARG)
		{
			cmd_tmp = cmd_cur;
			cmd_cur = cmd_cur->next;
			msh->in_type = cmd_tmp->type;
			free(cmd_tmp->token);
			free(cmd_tmp);
			msh->cmds = cmd_cur;
			return ;
		}
		cmd_tmp = cmd_cur;
		cmd_cur = cmd_cur->next;
		free(cmd_tmp->token);
		free(cmd_tmp);
		msh->cmds = cmd_cur;
	}
	msh->in_type = 0;
}

void	get_out_type(t_msh *msh)
{
	msh->out_type = 1;
}

void	exec_command(t_msh *msh)
{
	int	cmd_count;
	int	i;

	i = 0;
	if (!msh->cmds)
		return ;
	msh->fds = ft_calloc(2, sizeof(int *));
	cmd_count = get_cmd_count(msh->cmds);
	msh->pids = ft_calloc(cmd_count, sizeof(int *));
	if (!msh->pids)
		ft_exit(msh, 1);
	while (i < cmd_count)
	{
		if (i == 0)
			get_out_type(msh);
		else
			msh->out_type = 0;
		if (!cmd_is_builtin(msh, msh->cmds->token))
			get_cmd_path(msh);
		exec(msh, msh->cmds, get_cmd_args(msh), i);
		remove_command_from_msh(msh);
		i++;
	}
	i = cmd_count - 1;
	while (i >= 0)
	{
		waitpid(msh->pids[i], 0, 0);
		i--;
	}
	close(msh->fds[0]);
}
