/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 14:31:13 by tomoron           #+#    #+#             */
/*   Updated: 2024/04/24 15:17:24 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	signal_handler_interactive(int signum)
{
	if (signum == SIGINT)
	{
		g_return_code = 130;
		printf("%s%s^C\n", rl_prompt, rl_line_buffer);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	else if (signum == SIGQUIT)
		printf("%s%s", rl_prompt, rl_line_buffer);
}

void	*here_doc_variables(int write, void *data)
{
	static void	*variable;

	if (write)
		variable = data;
	else
		return (variable);
	return (0);
}

void	signal_handler_here_doc(int signum)
{
	t_msh	*msh;

	if (signum == SIGINT)
	{
		printf("%s%s^C\n", rl_prompt, rl_line_buffer);
		msh = here_doc_variables(0, 0);
		close(msh->in_fd);
		ft_exit(msh, 1);
	}
}

void	signal_handler_command(int signum)
{
	(void)signum;
}
