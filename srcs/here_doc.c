/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 17:44:32 by marde-vr          #+#    #+#             */
/*   Updated: 2024/03/25 19:14:46 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_tmp_file_name(t_msh *msh)
{
	int		i;
	char	*tmp_file_name;
	char	*res;
	char	*i_char;

	i = 0;
	tmp_file_name = ".tmp";
	i_char = ft_itoa(i);
	res = ft_strjoin_free(tmp_file_name, i_char, 2);
	if (!res)
		ft_exit(msh, 1);
	while (!access(res, F_OK))
	{
		free(res);
		i_char = ft_itoa(i);
		res = ft_strjoin_free(tmp_file_name, i_char, 2);
		if (!res)
			ft_exit(msh, 1);
		i++;
	}
	return (res);
}

int	contains_newline(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		if (str[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}

void	parse_var(t_msh *msh, char *line)
{
	char	*var_name;

	while(*line)
	{
		if(*line == '$')
		{
			var_name = get_var_name(line + 1);
			if(!var_name)
				return;
			line += ft_strlen(var_name);
			if(!*var_name)
				write(msh->in_fd, "$", 1);
			else if(!ft_strcmp(var_name, "?"))
				ft_putnbr_fd(g_return_code, msh->in_fd);
			else
				ft_putstr_fd(ft_get_env(msh->env, var_name), msh->in_fd); 
			free(var_name);
		}
		else
			write(msh->in_fd, line, 1);
		line++;
	}
}

void	get_here_doc_input(t_msh *msh, char *eof)
{
	char	*line;

	line = NULL;
	signal(SIGINT, signal_handler_here_doc);
	signal(SIGQUIT, signal_handler_here_doc);
	while (1)
	{
		free(line);
		line = readline("> ");
		if (!line)
		{
			ft_printf_fd(2, "\nminishell: warning: here-document delimited by");
			ft_printf_fd(2, " end-of-file, wanted %s", eof);
			break ;
		}
		if (line && !ft_strncmp(line, eof, ft_strlen(eof)))
			break ;
		parse_var(msh, line);
		write(msh->in_fd, "\n", 1);
	}
	free(line);
}

void	handle_here_doc(t_msh *msh, char *eof)
{
	char	*here_doc_file;
	int		pid;
	int		status;

	here_doc_file = get_tmp_file_name(msh);
	msh->in_fd = open(here_doc_file, O_CREAT | O_RDWR, 0644);
	if (msh->in_fd == -1)
	{
		perror("open");
		ft_exit(msh, 1);
	}
	pid = fork();
	if (pid == 0)
	{
		here_doc_variables(1, 0, msh);
		here_doc_variables(1, 1, here_doc_file);
		get_here_doc_input(msh, eof);
		close(msh->in_fd);
		free(here_doc_file);
		free(msh->fds[0]);
		ft_exit(msh, 0);
	}
	else
	{
		signal(SIGINT, signal_handler_command); 
		signal(SIGQUIT, signal_handler_command); 
		waitpid(pid, &status , 0);
		signal(SIGINT, signal_handler_interactive); 
		signal(SIGQUIT, signal_handler_interactive);
		close(msh->in_fd);
		if(WIFEXITED(status) && WEXITSTATUS(status))
			unlink(here_doc_file);
		msh->in_fd = open(here_doc_file, O_RDWR, 0644);
		free(here_doc_file);
		if (msh->in_fd == -1 && !(WIFEXITED(status) && WEXITSTATUS(status)))
			perror("open");
	}
}
