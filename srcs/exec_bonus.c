/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 13:50:14 by tomoron           #+#    #+#             */
/*   Updated: 2024/04/24 19:18:07 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	if (pid != 0)
		msh->pids[i] = pid;
	if (pid != 0)
		parent(msh, i, cmd_count, cmd_args);
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
	if (msh->here_doc_filename)
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
	{
		g_return_code = 0;
		return ;
	}
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
