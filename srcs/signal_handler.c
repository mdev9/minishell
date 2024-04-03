/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 14:31:13 by tomoron           #+#    #+#             */
/*   Updated: 2024/04/03 19:23:33 by tomoron          ###   ########.fr       */
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

void	*here_doc_variables(int write, int index, void *data)
{
	static void	*variables[2];

	if (write)
		variables[index] = data;
	else
		return (variables[index]);
	return (0);
}

void	signal_handler_here_doc(int signum)
{
	t_msh	*msh;
	char	*here_doc_file;

	if (signum == SIGINT)
	{
		printf("%s%s^C\n", rl_prompt, rl_line_buffer);
		msh = here_doc_variables(0, 0, 0);
		here_doc_file = here_doc_variables(0, 1, 0);
		close(msh->in_fd);
		free(here_doc_file);
		ft_exit(msh, 1);
	}
}

int	set_echoctl(int value)
{
	struct termios	t_p;

	ft_printf("echoctl value : %d\n",value);
	if (tcgetattr(1, &t_p))
	{
		ft_printf_fd(2, "minishell: an error occured while getting the local fl\
ags\n");
		return (1);
	}
	if (((t_p.c_lflag & ECHOCTL) != 0) == value)
		return (0);
	ft_printf("change\n");
	if (value)
		t_p.c_lflag = t_p.c_lflag | ECHOCTL;
	else
		t_p.c_lflag = t_p.c_lflag & (~ECHOCTL);
	if (tcsetattr(1, TCSANOW, &t_p))
	{
		ft_printf_fd(2, "minishell: an error occured while setting the local fl\
ags\n");
		return (1);
	}
	return (0);
}

void	signal_handler_command(int signum)
{
	(void)signum;
}
