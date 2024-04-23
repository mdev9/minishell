/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 21:47:15 by marde-vr          #+#    #+#             */
/*   Updated: 2024/04/23 16:41:42 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

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
		path = ft_strjoin(tmp, msh->tokens->value);
		if (!path)
			ft_exit(msh, 1);
		free(tmp);
		if (access(path, X_OK) != -1)
		{
			*found = 1;
			free(msh->tokens->value);
			msh->tokens->value = path;
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
		return ;
	}
	find_cmd_path(msh, paths, found);
	free_paths(paths);
}

char	*remove_path(char *token)
{
	while (ft_strchr(token, '/'))
		token++;
	return (token);
}

void	get_cmd_path(t_msh *msh)
{
	int	found;

	found = 0;
	if (ft_strchr(msh->tokens->value, '/'))
	{
		if (!file_access(msh, &found))
		{
			free(msh->tokens->value);
			msh->tokens->value = 0;
			return ;
		}
	}
	else
		get_path(msh, &found);
	if (!found)
	{
		ft_printf_fd(2, "%s: command not found\n", msh->tokens->value);
		free(msh->tokens->value);
		msh->tokens->value = 0;
		g_return_code = 127;
	}
}
