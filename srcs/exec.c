/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 14:12:49 by tomoron           #+#    #+#             */
/*   Updated: 2024/03/05 15:55:58 by marde-vr         ###   ########.fr       */
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
		|| !ft_strcmp(cmd_token, "pwd") || !ft_strcmp(cmd_token, "export")
		|| !ft_strcmp(cmd_token, "unset"))
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
	else if (!ft_strcmp(msh->cmds->token, "unset"))
		g_return_code = ft_unset(msh);
	else if (!ft_strcmp(msh->cmds->token, "alias"))
		g_return_code = alias(msh);
	else if (!ft_strcmp(msh->cmds->token, "unalias"))
		g_return_code = unalias(msh);
	else
		return (STDIN_FILENO);
	return (STDOUT_FILENO);
}

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
		if (msh->pids)
			free(msh->pids);
		if (msh->fds)
			free(msh->fds);
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
	if (cur_cmd->type == ARG)
		count++;
	while (cur_cmd->next)
	{
		if (cur_cmd->type == PIPE)
			break;
		cur_cmd = cur_cmd->next;
		if (cur_cmd->type == ARG)
			count++;
		else if (cur_cmd->type != PIPE)
			cur_cmd = cur_cmd->next;
	}
	return (count);
}

int	is_fd_open(int fd) // debug
{
	if (fcntl(fd, F_GETFL) == -1)
	{
		ft_printf_fd(2, "%d:closed\n", fd);
		return (0);
	}
	ft_printf_fd(2, "%d:open\n", fd);
	return (1);
}

void	redirect_input(t_msh *msh, int i)
{
	if (msh->in_type != PIPE)
	{
		if (dup2(msh->in_fd, 0) < 0)
			ft_exit(msh, 1);
		close(msh->in_fd);
	}
	else
	{
		//ft_printf_fd(2, "input_fd: %d\n", msh->fds[i - 1][0]);
		if (dup2(msh->fds[i - 1][0], 0) < 0)
			ft_exit(msh, 1);
	}
}

void	redirect_output(t_msh *msh, int i)
{
	if (msh->out_type != PIPE)
	{
		//ft_printf_fd(2, "output_fd: %d\n", msh->out_fd);
		if (dup2(msh->out_fd, 1) < 0)
			ft_exit(msh, 1);
	}
	else
	{
		if (dup2(msh->fds[i][1], 1) < 0)
			ft_exit(msh, 1);
	}
}

void	pipe_child(t_msh *msh, char **cmd_args, int i)
{
	if (msh->in_type != ARG)
	{
		//ft_printf_fd(2, "redirecting input of %s of type %d with fd %d\n", msh->cmds->token, msh->in_type, msh->in_fd);
		redirect_input(msh, i);
	}
	if (msh->out_type == PIPE || msh->out_type == RED_O || msh->out_type == RED_O_APP)
	{
		//ft_printf_fd(2, "redirecting output of %s of type %d with fd %d\n", msh->cmds->token, msh->out_type, msh->out_fd);
		redirect_output(msh, i);
	}
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
	if (msh->cmds->token && (!ft_strcmp(msh->cmds->token, "cd")
			|| !ft_strcmp(msh->cmds->token, "alias")
			|| !ft_strcmp(msh->cmds->token, "unalias")
			|| !ft_strcmp(msh->cmds->token, "exit") || exec_builtin(msh)))
	{
		while(i >= 0)
		{
			free(msh->fds[i]);
			i--;
		}
		free(cmd_args);
		ft_exit(msh, 1);
	}
	if (msh->cmds->token)
		execve(msh->cmds->token, cmd_args, env_to_char_tab(msh->env));
	close(0);
	close(1);
	close(2);
	while(i >= 0)
	{
		free(msh->fds[i]);
		i--;
	}
	free(cmd_args);
	ft_exit(msh, 1);
}

void	pipe_parent(t_msh *msh, int i, int cmd_count)
{
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

int	exec(t_msh *msh, char **cmd_args, int i, int cmd_count)
{
	pid_t	pid;

	if (i != cmd_count - 1)
	{
		if (pipe(msh->fds[i]) == -1)
		{
			perror("pipe");
			ft_exit(msh, 1);
		}
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		ft_exit(msh, 1);
	}
	if (pid == 0)
		pipe_child(msh, cmd_args, i);
	else
	{
		pipe_parent(msh, i, cmd_count);
		msh->pids[i] = pid;
		free(cmd_args);
	}
	return (0);
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
	//ft_printf_fd(2, "cmd: %s: args_count: %d\n", cur_cmd->token, args_count);
	i = 0;
	while (i < args_count)
	{
		if (cur_cmd->type == ARG)
		{
			cmd_args[i] = cur_cmd->token;
			//ft_printf_fd(2, "%s[%d] = %s\n", msh->cmds->token, i, cur_cmd->token);
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
		//while (cur_cmd->type != ARG)
		//{
		if (cur_cmd->type == PIPE)
		{
			cmd_tmp = cur_cmd;
			cur_cmd = cur_cmd->next;
			msh->in_type = cmd_tmp->type;
			free(cmd_tmp->token);
			free(cmd_tmp);
			msh->cmds = cur_cmd;
			return ;
		}
		cmd_tmp = cur_cmd;
		cur_cmd = cur_cmd->next;
		msh->in_type = cur_cmd->type;
		free(cmd_tmp->token);
		free(cmd_tmp);
		msh->cmds = cur_cmd;
		//}
		//cmd_tmp = cur_cmd;
		//cur_cmd = cur_cmd->next;
		//free(cmd_tmp->token);
		//free(cmd_tmp);
		//msh->cmds = cur_cmd;
	}
	msh->in_type = msh->cmds->type;
}

void	get_in_type(t_msh *msh, t_cmd *cmds)
{
	t_cmd	*cur_cmd;
	
	cur_cmd = cmds;
	while (cur_cmd && cur_cmd->next && cur_cmd->type == ARG)
		cur_cmd = cur_cmd->next;
	if (cur_cmd->type)
	{
		msh->in_type = cur_cmd->type;
		if (cur_cmd->type == HERE_DOC || cur_cmd->type == RED_I)
		{
			if (cur_cmd->type == HERE_DOC)
				handle_here_doc(msh, cur_cmd->next->token);
			if (cur_cmd->type == RED_I)
			{
				if (msh->in_fd != 0)
					close(msh->in_fd);
				msh->in_fd = open(cur_cmd->next->token, O_RDONLY | O_CREAT);
				//ft_printf_fd(2, "opened %s: %d\n", cur_cmd->next->token, msh->in_fd);
				if (msh->in_fd == -1 && !g_return_code)
				{
					ft_printf_fd(2, "minishell: %s: ", cur_cmd->next->token);
					perror("");
					// todo: cancel execution of all commands????????????????? idk
					g_return_code = 1;
				}
			}
			//ft_printf_fd(2, "cmd: %s\n", msh->cmds->token);
		}
	}
	while (cur_cmd && cur_cmd->next && cur_cmd->next->type == ARG)
		cur_cmd = cur_cmd->next;
	if (cur_cmd->next && (cur_cmd->next->type == HERE_DOC || cur_cmd->next->type == RED_I))
		get_in_type(msh, cur_cmd);
	//ft_printf_fd(2, "in_type: %d\n", msh->in_type);
}

void	get_out_type(t_msh *msh, t_cmd *cmds)
{
	t_cmd	*cur_cmd;

	msh->out_type = ARG;
	msh->out_fd = 0;
	//ft_printf_fd(2, "%s\n", cmds->token);
	cur_cmd = cmds;
	if (cmds->type && msh->cmds == cmds)
	{
		while (msh->cmds->type != ARG && msh->cmds->next->next)
			msh->cmds = msh->cmds->next->next;
	}
	while (cur_cmd && cur_cmd->next && (cur_cmd->type == ARG || cur_cmd->type > 3))
		cur_cmd = cur_cmd->next;
	if (!cur_cmd->type)
		msh->out_type = ARG;
	else
	{
		msh->out_type = cur_cmd->type;
		if (msh->out_type == RED_O)
			msh->out_fd = open(cur_cmd->next->token, O_CREAT | O_WRONLY | O_TRUNC,
					0644);
		if (msh->out_type == RED_O_APP)
			msh->out_fd = open(cur_cmd->next->token,
					O_CREAT | O_RDWR | O_APPEND, 0644);
		//if (msh->out_fd) // debug
		//	ft_printf_fd(2, "opened %s: %d\n", cur_cmd->next->token, msh->out_fd);
		if (msh->out_fd == -1)
		{
			g_return_code = 1;
			perror("open");
			return ;
		}
		if (cur_cmd->type != PIPE)
		{
			while (cur_cmd->next && cur_cmd->next->type == ARG)
				cur_cmd = cur_cmd->next;
			if (cur_cmd->next && (cur_cmd->next->type == RED_O || cur_cmd->next->type == RED_O_APP))
				get_out_type(msh, cur_cmd);
		}
	}
}

void	exec_command(t_msh *msh)
{
	int	cmd_count;
	int	i;

	i = 0;
	if (!msh->cmds)
		return ;
	cmd_count = get_cmd_count(msh->cmds);
	//ft_printf_fd(2, "cmd_count: %d\n", cmd_count);
	msh->fds = ft_calloc(cmd_count, sizeof(int **));
	msh->pids = ft_calloc(cmd_count, sizeof(int *));
	if (!msh->pids || !msh->fds)
		ft_exit(msh, 1);
	while (i < cmd_count)
	{
		g_return_code = 0;
		msh->fds[i] = ft_calloc(2, sizeof(int *));
		if (!msh->fds[i])
			ft_exit(msh, 1);
		get_in_type(msh, msh->cmds);
		if (!g_return_code) //maybe????
			get_out_type(msh, msh->cmds);
		//ft_printf_fd(2, "%d\n", msh->out_fd);
		//ft_printf_fd(2, "cmd: %s\n", msh->cmds->token);
		if (!cmd_is_builtin(msh, msh->cmds->token))
			get_cmd_path(msh);
		exec(msh, get_cmd_args(msh), i, cmd_count);
		remove_command_from_msh(msh);
		i++;
	}
	i = cmd_count - 1;
	while (i >= 0)
	{
		waitpid(msh->pids[i], 0, 0);
		i--;
	}
	i = 0;
	while (i < cmd_count)
	{
		free(msh->fds[i]);
		i++;
	}
	free(msh->fds);
	free(msh->pids);
}
