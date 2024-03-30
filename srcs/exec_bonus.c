/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 13:50:14 by tomoron           #+#    #+#             */
/*   Updated: 2024/03/30 17:29:07 by marde-vr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_command_bonus(t_msh *msh, char *cmd_str)
{
	t_cmd *cmds;

	(void)msh;
	printf("cmd : %s\n",cmd_str);
	cmds = parsing_bonus(cmd_str);
	printf("%p\n", cmds);
	msh->tokens = parse_command(cmd_str, msh->env);
	msh->cmds = cmds;
	print_parsed_cmd(cmds);
	exec_commands(msh);
}

int	exec(t_msh *msh, char **cmd_args, int i, int cmd_count)
{
	pid_t	pid;

	if (i != cmd_count - 1)
	{
		if (pipe(msh->fds[i]) == -1)
		{
			perror("pipe");
			ft_exit(msh, 1);
		}
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
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
	if (first_is_in_type(msh))
	{
		get_in_type(msh, msh->tokens);
		if (!g_return_code)
			get_out_type(msh, msh->tokens);
	}
	else
	{
		get_out_type(msh, msh->tokens);
		if (!g_return_code)
			get_in_type(msh, msh->tokens);
	}
	if (!cmd_is_builtin(msh, msh->cmds->value))
		get_cmd_path(msh);
	exec(msh, get_cmd_args(msh), i, cmd_count);
	remove_command_from_msh(msh);
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
		printf("Quit (core dumped)\n");
	i = 0;
	while (i < cmd_count)
	{
		free(msh->fds[i]);
		msh->fds[i] = 0;
		i++;
	}
	free(msh->fds);
	msh->fds = 0;
	free(msh->pids);
	msh->pids = 0;
	//signal(SIGINT, signal_handler_interactive); //enables ctrl-C
	signal(SIGQUIT, signal_handler_interactive);
	set_echoctl(0);
}

void	exec_commands(t_msh *msh)
{
	int	cmd_count;
	int	i;

	i = -1;
	ft_printf("yes\n");
	if (!msh->cmds)
		return ;
	cmd_count = get_cmd_count(msh->cmds);
	ft_printf("cmd_count: %d\n", cmd_count);
	msh->fds = ft_calloc(cmd_count, sizeof(int **));
	msh->pids = ft_calloc(cmd_count, sizeof(int *));
	if (!msh->pids || !msh->fds)
		ft_exit(msh, 1);
	while (++i < cmd_count)
		exec_command(msh, i, cmd_count);
	end_execution(msh, cmd_count);
}
