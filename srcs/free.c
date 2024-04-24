/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 10:51:13 by marde-vr          #+#    #+#             */
/*   Updated: 2024/04/24 10:51:25 by marde-vr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_fds(t_msh *msh)
{
	int	i;

	if (msh->fds)
	{
		i = 0;
		while (msh->fds[i])
		{
			free(msh->fds[i]);
			msh->fds[i] = 0;
			i++;
		}
		free(msh->fds);
		msh->fds = 0;
	}
}

void	free_msh(t_msh *msh)
{
	if (msh)
	{
		free_env(msh->env);
		free(msh->pids);
		free_cmd(msh->cmds_head);
		free_fds(msh);
		free(msh->here_doc_filename);
		free_token(msh->tokens);
		free(msh);
	}
}
