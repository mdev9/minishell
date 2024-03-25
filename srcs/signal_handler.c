/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 14:31:13 by tomoron           #+#    #+#             */
/*   Updated: 2024/03/25 19:10:39 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	signal_handler_interactive(int signum)
{
	if (signum == 2)
	{
		g_return_code = 130;
		ft_putstr_fd("\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	if (signum == 3)
	{
		rl_redisplay();
		ft_putstr_fd("filsdeup", 1);
	}
}

void	*here_doc_variables(int write, int index, void *data)
{
	static void *variables[2];

	if (write)
		variables[index] = data;
	else
		return (variables[index]);
	return(0);
}

void	signal_handler_here_doc(int signum)
{
	t_msh	*msh;
	char	*here_doc_file;

	if(signum == 2)
	{
		write(1,"\n",1);
		msh = here_doc_variables(0, 0, 0);
		here_doc_file = here_doc_variables(0, 1, 0);
		close(msh->in_fd);
		free(here_doc_file);
		free(msh->fds[0]);
		ft_exit(msh, 1);
	}
}

void	signal_handler_command(int signum)
{
	(void)signum;
}
