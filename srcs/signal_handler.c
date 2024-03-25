/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 14:31:13 by tomoron           #+#    #+#             */
/*   Updated: 2024/03/25 20:39:21 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	signal_handler_interactive(int signum)
{
	if (signum == SIGINT)
	{
		g_return_code = 130;
		printf("%s%s^C\n",rl_prompt, rl_line_buffer);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	else if (signum == SIGQUIT)
		printf("%s%s", rl_prompt, rl_line_buffer);
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

	if(signum == SIGINT)
	{
		printf("%s%s^C\n",rl_prompt, rl_line_buffer);
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
	if(signum == SIGQUIT)
		printf("^\\Quit (core dumped)\n");
}
