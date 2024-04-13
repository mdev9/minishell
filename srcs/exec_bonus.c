/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 13:50:14 by tomoron           #+#    #+#             */
/*   Updated: 2024/04/13 16:22:58 by babonnet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	get_redirections(t_msh *msh, t_cmd *cmds)
{
	if (first_is_in_type(cmds))
	{
		get_in_type(msh, cmds);
		if (!g_return_code)
			get_out_type(msh, cmds);
	}
	else
	{
		get_out_type(msh, cmds);
		if (!g_return_code)
			get_in_type(msh, cmds);
	}
}

void	exec_command_bonus(t_msh *msh, char *cmd_str)
{
	t_cmd	*cmds;
	t_cmd	*tmp;

	cmds = parsing_bonus(cmd_str);
	print_parsed_cmd(cmds); // debug
	tmp = check_cmds_syntax(cmds);
	if (tmp)
	{
		print_syntax_error_bonus(tmp);
		printf("error\n"); // debug
		free_cmd(cmds);
		return ;
	}
	while (cmds)
	{
		if ((cmds->cmd_type == AND && !g_return_code) || (cmds->cmd_type == OR
				&& g_return_code) || !is_operand_type(cmds))
		{
			if (is_operand_type(cmds))
				cmds = cmds->next;
			msh->tokens = parse_command(cmds->value, msh->env);
			msh->cmds = cmds;
			get_redirections(msh, cmds);
			print_msh_struct(msh);           // debug
			print_parsed_token(msh->tokens); // debug
			exec_commands(msh);
			msh->in_fd = 0;
			msh->out_fd = 0;
		}
		while (cmds && (is_cmd_type(cmds) || cmds->cmd_type == PIPE
				|| is_output_type(cmds) || is_input_type(cmds)))
			cmds = cmds->next;
	}
}

int	exec(t_msh *msh, char **cmd_args, int i, int cmd_count)
{
	pid_t	pid;

	if (i != cmd_count - 1)
	{
		printf("pipe\n");
		if (pipe(msh->fds[i]) == -1)
		{
			perror("minishell: pipe");
			ft_exit(msh, 1);
		}
		ft_printf_fd(2, "msh->fds[%d][0]: %d, msh->fds[%d][1]: %d\n", i, msh->fds[i][0], i, msh->fds[i][1]);
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
	g_return_code = 0;
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
		cmds = cmds->next;
	}
	return (nb);
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
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGQUIT)
		printf("Quit\n");
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
	printf("cmd_count : %d\n", cmd_count);
	msh->fds = ft_calloc(cmd_count, sizeof(int **));
	msh->pids = ft_calloc(cmd_count, sizeof(int *));
	if (!msh->pids || !msh->fds)
		ft_exit(msh, 1);
	i = 0;
	while (i < cmd_count)
	{
		if (i != 0)
			get_redirections(msh, msh->cmds);
		exec_command(msh, i, cmd_count);
		free(msh->fds[i]);
		i++;
	}
	end_execution(msh, cmd_count);	
}

