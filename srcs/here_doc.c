/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 17:44:32 by marde-vr          #+#    #+#             */
/*   Updated: 2024/05/22 13:48:39 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	get_here_doc_input(t_msh *msh, char *eof)
{
	char	*line;

	line = NULL;
	while (1)
	{
		free(line);
		line = readline("> ");
		if (!line)
		{
			ft_printf_fd(2, "\nminishell: warning: here-document delimited by");
			ft_printf_fd(2, " end-of-file, (wanted `%s')\n", eof);
			break ;
		}
		if (line && !ft_strcmp(line, eof))
			break ;
		parse_var(msh, line);
		write(msh->in_fd, "\n", 1);
	}
	free(line);
}

void	here_doc_child(t_msh *msh, char *eof)
{
	rl_catch_signals = 1;
	here_doc_variables(1, msh);
	set_echoctl(0);
	signal(SIGINT, signal_handler_here_doc);
	signal(SIGQUIT, SIG_IGN);
	get_here_doc_input(msh, eof);
	close(msh->in_fd);
	close_all_pipes(msh);
	ft_exit(msh, 0);
}

void	here_doc_signal(t_msh *msh, int child_pid, char *here_doc_file)
{
	int	status;

	signal(SIGINT, signal_handler_command);
	signal(SIGQUIT, signal_handler_here_doc);
	waitpid(child_pid, &status, 0);
	set_echoctl(msh->echoctl);
	signal(SIGINT, signal_handler_interactive);
	signal(SIGQUIT, signal_handler_interactive);
	if (msh->in_fd > 2)
		close(msh->in_fd);
	if (WIFEXITED(status) && WEXITSTATUS(status))
	{
		unlink(here_doc_file);
		msh->in_fd = -2;
		g_return_code = 130;
		return ;
	}
	msh->in_fd = open(here_doc_file, O_RDWR, 0644);
	if (msh->in_fd == -1 && !(WIFEXITED(status) && WEXITSTATUS(status)))
		perror("open");
}

void	handle_here_doc(t_msh *msh, char *eof)
{
	char	*here_doc_file;
	int		pid;

	here_doc_file = get_tmp_file_name(msh);
	if (msh->here_doc_filename)
	{
		if (msh->in_fd)
			close(msh->in_fd);
		unlink(msh->here_doc_filename);
		free(msh->here_doc_filename);
	}
	msh->here_doc_filename = here_doc_file;
	msh->in_fd = open(here_doc_file, O_CREAT | O_RDWR, 0644);
	if (msh->in_fd == -1)
	{
		perror("open");
		ft_exit(msh, 1);
	}
	pid = fork();
	if (pid == 0)
		here_doc_child(msh, eof);
	else
		here_doc_signal(msh, pid, here_doc_file);
}
