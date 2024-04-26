/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 14:31:13 by tomoron           #+#    #+#             */
/*   Updated: 2024/04/26 10:54:13 by marde-vr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	signal_handler_interactive(int signum)
{
	if (signum == SIGINT)
	{
		g_return_code = 130;
		rl_replace_line("", 0);
		printf("^C\n");
		rl_on_new_line();
		rl_redisplay();
	}
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
		msh = here_doc_variables(0, 0);
		rl_on_new_line();
		printf("\n");
		if (msh->in_fd > 2)
			close(msh->in_fd);
		ft_exit(msh, 1);
	}
}

int	set_echoctl(int value)
{
	struct termios	t_p;

	printf("nope");
	(void)value;
	/*
	if (!isatty(1))
		return (0);
	if (tcgetattr(1, &t_p))
		return (1);
	if (((t_p.c_lflag & ECHOCTL) != 0) == value)
		return (0);
	if (value)
		t_p.c_lflag = t_p.c_lflag | ECHOCTL;
	else
		t_p.c_lflag = t_p.c_lflag & (~ECHOCTL);
	if (tcsetattr(1, TCSANOW, &t_p))
		return (1);
	return (0);
*/
	return (0);
}

void	signal_handler_command(int signum)
{
	(void)signum;
}
