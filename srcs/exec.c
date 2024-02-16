/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 14:12:49 by tomoron           #+#    #+#             */
/*   Updated: 2024/02/17 00:26:16 by marde-vr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_builtin(t_cmd *parsed_cmd, t_env *env)
{

	if (!ft_strcmp(parsed_cmd->token, "echo"))
		g_return_code = echo(parsed_cmd->next);
	else if (!ft_strcmp(parsed_cmd->token, "ret"))
		g_return_code = ft_atoi(parsed_cmd->next->token);
	else if (!ft_strcmp(parsed_cmd->token, "env"))
		g_return_code = print_env(env);
	else if (!ft_strcmp(parsed_cmd->token, "exit"))
		exit_bt(parsed_cmd, env);
	else if (!ft_strcmp(parsed_cmd->token, "pwd"))
		g_return_code = pwd();
	else if (!ft_strcmp(parsed_cmd->token, "cd"))
		g_return_code = cd(parsed_cmd);
	else
		return (STDIN_FILENO);
	return (STDOUT_FILENO);
}

void	ft_exit(t_cmd *parsed_cmd, t_env *env, int exit_code)
{
	free_cmd(parsed_cmd);
	free_env(env);
	exit(exit_code);
}

int	get_args_count(t_cmd *parsed_cmd)
{
	int		count;
	t_cmd	*cur_cmd;

	count = 0;
	cur_cmd = parsed_cmd;
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
		ft_printf_fd(2, "pipex: error: PATH not found\n");
		return (0);
	}
	return (ft_split(cur_env_var->value, ':'));
}

void	find_cmd_path(t_cmd *cmd, t_env *env, char **paths, int *found)
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
			ft_exit(cmd, env, 1);
		path = ft_strjoin(tmp, cmd->token);
		if (!path)
			ft_exit(cmd, env, 1);
		free(tmp);
		if (access(path, X_OK) != -1)
		{
			*found = 1;
			free(cmd->token);
			cmd->token = path;
			break ;
		}
		free(path);
		i++;
	}
}

void	get_cmd_path(t_cmd *cmd, t_env *env)
{
	char	**paths;
	int		found;

	found = 0;
	if (ft_strchr(cmd->token, '/') && access(cmd->token, X_OK) != -1)
		found = 1;
	else
	{
		paths = split_paths_from_env(env);
		if (!paths)
			ft_exit(cmd, env, 1);
		find_cmd_path(cmd, env, paths, &found);
	}
	if (!found)
	{
		ft_printf_fd(2, "%s: command not found\n", cmd->token);
		free(cmd->token);
		cmd->token = 0;
	}
}

void	exec_command(t_cmd *parsed_cmd, t_env *env)
{
	t_cmd	*cur_cmd;
	int		args_count;
	char	**cmd_args;
	int		i;

	if (!parsed_cmd || exec_builtin(parsed_cmd, env))
		return ;
	cur_cmd = parsed_cmd;
	args_count = get_args_count(parsed_cmd);
	cmd_args = ft_calloc(args_count + 1, sizeof(char *));
	if (!cmd_args)
		ft_exit(parsed_cmd, env, 1);
	i = 0;
	get_cmd_path(parsed_cmd, env);
	while (i < args_count)
	{
		cmd_args[i] = cur_cmd->token;
		cur_cmd = cur_cmd->next;
		i++;
	}
	pid_t	pid;
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		ft_exit(parsed_cmd, env, 1);
	}
	if (pid == 0)
	{
		if (parsed_cmd->token)
			execve(parsed_cmd->token, cmd_args, env_to_char_tab(env));
		ft_exit(parsed_cmd, env, 1);
	}
	else
	{
		rl_redisplay();
		if (waitpid(pid, 0, 0) < 0)
			ft_exit(parsed_cmd, env, 1);
	}
	//if (cur_cmd->type == PIPE)
	//	exec_command(cur_cmd->next, env);
}
