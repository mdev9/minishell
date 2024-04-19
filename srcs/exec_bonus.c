/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 13:50:14 by tomoron           #+#    #+#             */
/*   Updated: 2024/04/19 17:59:31 by tomoron          ###   ########.fr       */
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
	while(cmd)
	{
		while(cmd && !is_operand_type(cmd))
			cmd = cmd->next;
		if(cmd && cmd->cmd_type == AND && !g_return_code)
			return(cmd->next);
		if(cmd && cmd->cmd_type == OR && g_return_code)
			return(cmd->next);
		if(cmd)
			cmd = cmd->next;
	}
	return(0);
}

void	exec_command_bonus(t_msh *msh, char *cmd_str)
{
	t_cmd	*cmds;
	t_cmd	*tmp;

	if(!cmd_str)
		return ;
	cmds = parsing_bonus(cmd_str);
	//print_parsed_cmd(cmds); // debug
	tmp = check_cmds_syntax(cmds);
	if (tmp)
	{
		print_syntax_error_bonus(tmp);
		//printf("error\n"); // debug
		free_cmd(cmds);
		return ;
	}
	while (cmds)
	{
		print_parsed_cmd(cmds); // debug
		msh->tokens = parse_cmds_to_token(cmds, msh->env);
		msh->cmds = cmds;
		//print_msh_struct(msh);           // debug
	//	print_parsed_token(msh->tokens); // debug
		exec_commands(msh);
		msh->in_fd = 0;
		msh->out_fd = 0;
		cmds = get_next_command(cmds);
	}
}

int	exec(t_msh *msh, char **cmd_args, int i, int cmd_count)
{
	pid_t	pid;

	if (i != cmd_count - 1)
	{
		//fprintf(stderr, "piping %d", i);
		if (pipe(msh->fds[i]) == -1)
		{
			perror("minishell: pipe");
			ft_exit(msh, 1);
		}
		//fprintf(stderr, "pipe: msh->fds[%d][0]: %d, msh->fds[%d][1]: %d\n", i, msh->fds[i][0], i, msh->fds[i][1]);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
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
	//g_return_code = 0;
	msh->fds[i] = ft_calloc(2, sizeof(int *));
	if (!msh->fds[i])
		ft_exit(msh, 1);
	if (!cmd_is_builtin(msh, msh->tokens->value))
		get_cmd_path(msh);
	exec(msh, get_cmd_args(msh), i, cmd_count);
	remove_command_from_msh(msh);
}

int	get_cmd_count(t_cmd *cmds)
{
	int	nb;

	nb = 0;
	while (cmds && !is_operand_type(cmds))
	{
		if (is_cmd_type(cmds))
			nb++;
		while(cmds && (is_output_type(cmds) || is_input_type(cmds) || is_cmd_type(cmds)))
			cmds=cmds->next;
		if(cmds && cmds->cmd_type == PIPE)
			cmds = cmds->next;
	}
	return (nb);
}

void	print_signaled(int status)
{
	int signal;
	int print;
	static const char *sigmsg[] = {0, "Hangup",0, "Quit", "Illegal instruction",\
	"Trace/breakpoint trap", "Aborted", "Bus error",\
	"Floating point exception", "Killed", "User defined signal 1",\
	"Segmentation fault","User defined signal 2", 0, "Alarm clock",\
	"Terminated","Stack fault" , 0 , 0, "Stopped", "Stopped","Stopped",\
	"Stopped",0 , "CPU time limit exceeded","File size limit exceeded", \
	"Virtual time expired", "Profiling timer expired",\
	"I/O possible", "Power failure", "Bad system call"};

	signal = WTERMSIG(status);
	print = 0;
	if(signal < 31 && sigmsg[signal])
	{
		ft_putstr_fd((char *)sigmsg[signal], 2);
		print = 1;
	}
	if(signal >= 34 && signal <= 64)
	{
		ft_putstr_fd("Real-time signal ", 2);
		ft_putnbr_fd(signal - 34, 2);
		print = 1;

	}
	if(print)
		ft_putstr_fd("\n", 2);
}

void	end_execution(t_msh *msh, int cmd_count)
{
	int	i;
	int	status;

	i = 0;
	while (i < cmd_count)
			waitpid(msh->pids[i++], &status, 0);
	if (!g_return_code && WIFEXITED(status))
		g_return_code = WEXITSTATUS(status);
	if (WIFSIGNALED(status))
		print_signaled(status);
	// TODO: (core dumped) WCOREDUMP
	free(msh->pids);
	msh->pids = 0;
	free(msh->fds);
	// signal(SIGINT, signal_handler_interactive); //enables ctrl-C
	signal(SIGQUIT, signal_handler_interactive);
	set_echoctl(0);
}


void	exec_commands(t_msh *msh)
{
	int	cmd_count;
	int	i;

	if (!msh->tokens)
		return ;
	cmd_count = get_cmd_count(msh->cmds);
	//printf("cmd_count : %d\n", cmd_count);
	msh->fds = ft_calloc(cmd_count, sizeof(int **));
	msh->pids = ft_calloc(cmd_count, sizeof(int *));
	if (!msh->pids || !msh->fds)
		ft_exit(msh, 1);
	i = 0;
	while (i < cmd_count)
	{
		get_redirections(msh, msh->cmds);
		//fprintf(stderr, "command: %s, in_type: %d, out_type: %d\n", msh->cmds->value, msh->in_type, msh->out_type);
		exec_command(msh, i, cmd_count);
		//free(msh->fds[i]);
		i++;
	}
	end_execution(msh, cmd_count);	
}

