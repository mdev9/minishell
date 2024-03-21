/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 18:19:26 by marde-vr          #+#    #+#             */
/*   Updated: 2024/03/21 13:21:52 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_msh(t_msh *msh)
{
	int cmd_count = 0;
	if (msh)
	{
		free_env(msh->env);
		free_alias(msh->aliases);
		free(msh->pids);
		if(msh->fds)
		{
			cmd_count = get_cmd_count(msh->cmds);
			while(cmd_count)
			{
				free(msh->fds[cmd_count - 1]);
				cmd_count--;
			}
		}
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
	if (open(msh->cmds->token, O_DIRECTORY) != -1)
	{
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
