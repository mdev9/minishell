/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 13:50:14 by tomoron           #+#    #+#             */
/*   Updated: 2024/04/23 17:07:41 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	get_redirections(t_msh *msh, t_cmd *cmds)
{
	msh->in_type = 0;
	msh->out_type = 0;
	if (first_is_in_type(cmds))
	{
		if (!get_in_type(msh, cmds))
			get_out_type(msh, cmds);
	}
	else
	{
		if (!get_out_type(msh, cmds))
			get_in_type(msh, cmds);
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

void	exec_command_bonus(t_msh *msh, char *cmd_str)
{
	t_cmd	*cmds;
	t_cmd	*tmp;

	if (!cmd_str)
		return ;
	cmds = parsing_bonus(cmd_str);
	tmp = check_cmds_syntax(cmds, msh->env);
	if (tmp)
	{
		print_syntax_error_bonus(tmp);
		free_cmd(cmds);
		return ;
	}
	msh->cmds_head = cmds;
	while (cmds)
	{
		msh->tokens = parse_cmds_to_token(cmds, msh->env);
		msh->cmds = cmds;
		exec_commands(msh);
		msh->in_fd = 0;
		msh->out_fd = 0;
		cmds = get_next_command(cmds);
	}
	free_cmd(msh->cmds_head);
	msh->cmds_head = 0;
}

int	exec(t_msh *msh, char **cmd_args, int i, int cmd_count)
{
	pid_t	pid;

	if (i != cmd_count - 1)
	{
		if (pipe(msh->fds[i]) == -1)
		{
			perror("minishell: pipe");
			free(cmd_args);
			ft_exit(msh, 1);
		}
	}
	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		free(cmd_args);
		ft_exit(msh, 1);
	}
	if (pid == 0)
		child(msh, cmd_args, i);
	else
	{
		parent(msh, i, cmd_count);
		msh->pids[i] = pid;
		free(cmd_args);
	}
	return (0);
}

void	exec_command(t_msh *msh, int i, int cmd_count)
{
	g_return_code = 0;
	msh->fds[i] = ft_calloc(2, sizeof(int *));
	if (!msh->fds[i])
		ft_exit(msh, 1);
	if (msh->tokens && !cmd_is_builtin(msh, msh->tokens->value))
		get_cmd_path(msh);
	if ((msh->tokens && msh->tokens->value) || is_parenthesis(msh->cmds))
		exec(msh, get_cmd_args(msh), i, cmd_count);
	remove_command_from_msh(msh);
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

int		is_parenthesis(t_cmd *cmd)
{
	if(!cmd)
		return(0);
	return(cmd->cmd_type == PAREN || (cmd->cmd_type == PIPE && cmd->next->cmd_type == PAREN));
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

void	end_execution(t_msh *msh, int cmd_count)
{
	int	i;
	int	status;

	i = 0;
	status = 0;
	while (i < cmd_count)
		waitpid(msh->pids[i++], &status, 0);
	if (!g_return_code && WIFEXITED(status))
		g_return_code = WEXITSTATUS(status);
	if (WIFSIGNALED(status))
		print_signaled(status);
	if(msh->here_doc_filename)
	{
		unlink(msh->here_doc_filename);
		free(msh->here_doc_filename);
		msh->here_doc_filename = 0;
	}
	free(msh->pids);
	free_fds(msh);
	msh->pids = 0;
	free(msh->fds);
	signal(SIGINT, signal_handler_interactive);
	signal(SIGQUIT, signal_handler_interactive);
	set_echoctl(0);
}

void	exec_commands(t_msh *msh)
{
	int	cmd_count;
	int	i;

	if (!msh->tokens && !is_parenthesis(msh->cmds))
		return ;
	cmd_count = get_cmd_count(msh->cmds);
	msh->fds = ft_calloc(cmd_count + 1, sizeof(int **));
	msh->pids = ft_calloc(cmd_count, sizeof(int *));
	if (!msh->pids || !msh->fds)
		ft_exit(msh, 1);
	i = 0;
	while (i < cmd_count)
	{
		get_redirections(msh, msh->cmds);
		exec_command(msh, i, cmd_count);
		i++;
	}
	end_execution(msh, cmd_count);
}
