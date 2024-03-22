/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 21:47:15 by marde-vr          #+#    #+#             */
/*   Updated: 2024/03/22 15:45:44 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

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
		return (0);
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

void	free_paths(char **paths)
{
	int	i;

	i = 0;
	while (paths && paths[i])
	{
		free(paths[i]);
		i++;
	}
	free(paths);
}

void	get_path(t_msh *msh, int *found)
{
	char	**paths;

	paths = split_paths_from_env(msh->env);
	if (!paths)
	{
		free_paths(paths);
		return;
	}
	find_cmd_path(msh, paths, found);
	free_paths(paths);
}

char	*remove_path(char *token)
{
	int	i;

	i = 0;
	while (ft_strchr(token, '/'))
		token++;
	return (token);
}

void	get_cmd_path(t_msh *msh)
{
	int	found;

	found = 0;
	if (ft_strchr(msh->cmds->token, '/'))
	{
		if (!file_access(msh, &found))
			return ;
	}
	else
		get_path(msh, &found);
	if (!found)
	{
		ft_printf_fd(2, "%s: command not found\n", msh->cmds->token);
		free(msh->cmds->token);
		msh->cmds->token = 0;
		g_return_code = 127;
	}
}
