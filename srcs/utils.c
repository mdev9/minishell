/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 18:19:26 by marde-vr          #+#    #+#             */
/*   Updated: 2024/03/23 19:21:13 by marde-vr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_msh(t_msh *msh)
{
	if (msh)
	{
		free_env(msh->env);
		free_alias(msh->aliases);
		free(msh->pids);
		free(msh->fds);
		free_cmd(msh->cmds);
		free(msh);
	}
}

void	ft_exit(t_msh *msh, int exit_code)
{
	free_msh(msh);
	exit(exit_code);
}

int	file_access(t_msh *msh, int *found)
{
	int	fd;

	fd = open(msh->cmds->token, O_DIRECTORY);
	if (fd != -1)
	{
		close(fd);
		ft_printf_fd(2, "minishell: %s: Is a directory\n", msh->cmds->token);
		g_return_code = 126;
		return (0);
	}
	if (access(msh->cmds->token, X_OK) != -1)
		*found = 1;
	else
	{
		ft_printf_fd(2, "minishell: %s: ", msh->cmds->token);
		perror("");
		g_return_code = 127;
		if (access(msh->cmds->token, F_OK) != -1)
			g_return_code = 126;
		return (0);
	}
	return (1);
}
