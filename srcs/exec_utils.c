/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 10:46:28 by marde-vr          #+#    #+#             */
/*   Updated: 2024/04/24 14:05:18 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	get_redirections(t_msh *msh, t_cmd *cmds)
{
	msh->in_type = 0;
	msh->out_type = 0;
	if (first_is_in_type(cmds))
	{
		if (!get_in_type(msh, cmds, cmds, 1))
			get_out_type(msh, cmds);
	}
	else
	{
		if (!get_out_type(msh, cmds))
			get_in_type(msh, cmds, cmds, 1);
	}
}

t_cmd	*get_next_command(t_cmd *cmd)
{
	while (cmd)
	{
		while (cmd && !is_operand_type(cmd))
			cmd = cmd->next;
		if (cmd && cmd->cmd_type == AND && !g_return_code)
			return (cmd->next);
		if (cmd && cmd->cmd_type == OR && g_return_code)
			return (cmd->next);
		if (cmd)
			cmd = cmd->next;
	}
	return (0);
}

int	get_cmd_count(t_cmd *cmds)
{
	int	nb;

	nb = 0;
	while (cmds && !is_operand_type(cmds))
	{
		while (cmds && (is_output_type(cmds) || is_input_type(cmds)))
			cmds = cmds->next;
		if (is_cmd_type(cmds))
			nb++;
		while (cmds && (is_output_type(cmds) || is_input_type(cmds)
				|| is_cmd_type(cmds)))
			cmds = cmds->next;
		if (cmds && cmds->cmd_type == PIPE)
			cmds = cmds->next;
	}
	return (nb);
}

int	is_parenthesis(t_cmd *cmd)
{
	if (!cmd)
		return (0);
	return (cmd->cmd_type == PAREN || (cmd->cmd_type == PIPE
			&& cmd->next->cmd_type == PAREN));
}

void	print_signaled(int status)
{
	int					signal;
	static const char	*sigmsg[] = {0, "Hangup", 0, "Quit", "Illegal \
		instruction", "Trace/breakpoint trap", "Aborted", "Bus error",
		"Floating point exception", "Killed", "User defined signal 1",
		"Segmentation fault (skill issue)", "User defined signal 2", 0,
		"Alarm clock", "Terminated", "Stack fault", 0, 0, "Stopped", "Stopped",
		"Stopped", "Stopped", 0, "CPU time limit exceeded",
		"File size limit exceeded", "Virtual time expired",
		"Profiling timer expired", "I/O possible", "Power failure",
		"Bad system call"};

	signal = WTERMSIG(status);
	if (signal < 31 && sigmsg[signal])
	{
		ft_putstr_fd((char *)sigmsg[signal], 2);
	}
	if (signal >= 34 && signal <= 64)
	{
		ft_putstr_fd("Real-time signal ", 2);
		ft_putnbr_fd(signal - 34, 2);
	}
	if (WCOREDUMP(status))
		ft_putstr_fd(" (core dumped)", 2);
	ft_putstr_fd("\n", 2);
	g_return_code = signal + 128;
}
