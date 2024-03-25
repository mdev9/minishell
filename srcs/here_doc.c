/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 17:44:32 by marde-vr          #+#    #+#             */
/*   Updated: 2024/03/25 12:57:12 by marde-vr         ###   ########.fr       */
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
			ft_printf_fd(2, " end-of-file, wanted %s", eof);
			break ;
		}
		if (line && !ft_strncmp(line, eof, ft_strlen(eof)))
			break ;
		write(msh->in_fd, line, ft_strlen(line));
		write(msh->in_fd, "\n", 1);
	}
	free(line);
}

void	handle_here_doc(t_msh *msh, char *eof)
{
	char	*here_doc_file;
	int		pid;

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
		get_here_doc_input(msh, eof);
		close(msh->in_fd);
		free(here_doc_file);
		ft_exit(msh, 0);
		//exit(0);
	}
	else
	{
		waitpid(pid, 0 , 0);
		close(msh->in_fd);
		msh->in_fd = open(here_doc_file, O_RDWR, 0644);
		free(here_doc_file);
		if (msh->in_fd == -1)
		{
			perror("open");
			ft_printf_fd(2, "exiting\n");
			ft_exit(msh, 1);
		}
	}
}
