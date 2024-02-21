/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 14:12:49 by tomoron           #+#    #+#             */
/*   Updated: 2024/02/21 19:11:20 by marde-vr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_builtin(t_msh *msh)
{
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

char	**split_paths_from_env(t_env *env)
{
	t_env	*cur_env_var;
	int		path_in_envp;

	path_in_envp = 0;
	cur_env_var = env;
	while (cur_env_var->next != 0)
	{
		if (!ft_strcmp(cur_env_var->name, "PATH"))
		{
			path_in_envp = 1;
			break ;
		}
		cur_env_var = cur_env_var->next;
	}
	if (!path_in_envp)
	{
		ft_printf_fd(2, "msh: error: PATH not found\n");
		return (0);
	}
	return (ft_split(cur_env_var->value, ':'));
}

void	find_cmd_path(t_msh *msh, char **paths, int *found)
{
	char	*tmp;
	char	*path;
	int		i;

	i = 0;
	while (paths[i])
	{
		path = paths[i];
		tmp = ft_strjoin(path, "/");
		if (!tmp)
			ft_exit(msh, 1);
		path = ft_strjoin(tmp, msh->cmds->token);
		if (!path)
			ft_exit(msh, 1);
		free(tmp);
		if (access(path, X_OK) != -1)
		{
			*found = 1;
			free(msh->cmds->token);
			msh->cmds->token = path;
			break ;
		}
		free(path);
		i++;
	}
}

void	get_cmd_path(t_msh *msh)
{
	char	**paths;
	int		found;

	found = 0;
	if (ft_strchr(msh->cmds->token, '/')
		&& access(msh->cmds->token, X_OK) != -1)
		found = 1;
	else
	{
		paths = split_paths_from_env(msh->env);
		if (!paths)
			ft_exit(msh, 1);
		find_cmd_path(msh, paths, &found);
	}
	if (!found)
	{
		ft_printf_fd(2, "%s: command not found\n", msh->cmds->token);
		free(msh->cmds->token);
		msh->cmds->token = 0;
	}
}

void	redirect_input(t_msh *msh)
{
	if (dup2(msh->fds[0], 0) < 0)
		ft_exit(msh, 1);
}

void	redirect_output(t_msh *msh)
{
	if (dup2(msh->fds[1], 1) < 0)
		ft_exit(msh, 1);
	close(msh->fds[1]);
}

void	pipe_child(t_msh *msh, char **cmd_args)
{
	redirect_input(msh);
	redirect_output(msh);
	close(msh->fds[0]);
	close(msh->fds[1]);
	execve(msh->cmds->token, cmd_args, env_to_char_tab(msh->env));
	close(0);
	close(1);
	close(2);
	ft_exit(msh, 1);
}

void	pipe_parent(t_msh *msh)
{
	close(msh->fds[0]);
	close(msh->fds[1]);
}

int	exec(t_msh *msh, t_cmd *cmd, char **cmd_args)
{
	pid_t	pid;

	if (msh->cmds != cmd)
	{
		if (pipe(msh->fds) == -1)
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
		pipe_child(msh, cmd_args);
	else
	{
		pipe_parent(msh);
		rl_redisplay();
		if (waitpid(pid, 0, 0) < 0)
			ft_exit(msh, 1);
	}
	return (0);
}

void	exec_command(t_msh *msh)
{
	t_cmd	*cur_cmd;
	int		args_count;
	char	**cmd_args;
	int		i;
	

	if (!msh->cmds || exec_builtin(msh))
		return ;
	cur_cmd = msh->cmds;
	args_count = get_args_count(msh->cmds);
	msh->fds = ft_calloc(2, sizeof(int *));
	cmd_args = ft_calloc(args_count + 1, sizeof(char *));
	if (!cmd_args || !msh->fds)
		ft_exit(msh, 1);
	i = 0;
	get_cmd_path(msh);
	while (i < args_count)
	{
		cmd_args[i] = cur_cmd->token;
		cur_cmd = cur_cmd->next;
		i++;
	}

	exec(msh, msh->cmds, cmd_args);
	//free(msh->fds);
	/*
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		ft_exit(msh, 1);
	}
	if (pid == 0)
	{
		if (msh->cmds->token)
			execve(msh->cmds->token, cmd_args, env_to_char_tab(msh->env));
		ft_exit(msh, 1);
	}
	else
	{
		rl_redisplay();
		if (waitpid(pid, 0, 0) < 0)
			ft_exit(msh, 1);
	}*/
	//if (cur_cmd->type == PIPE)
	//	exec_command(cur_cmd->next, env);
}
