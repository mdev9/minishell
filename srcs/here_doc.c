/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 20:20:31 by marde-vr          #+#    #+#             */
/*   Updated: 2024/02/28 17:39:18 by marde-vr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_tmp_file_name(t_msh *msh/*, int argc, char **argv*/)
{
	int		i;
	char	*tmp_file_name;
	char	*res;
	char	*i_char;

	i = 0;
	tmp_file_name = ".tmp";
	i_char = ft_itoa(i);
	res = ft_strjoin(tmp_file_name, i_char);
	if (!res)
		ft_exit(msh, 1);
	free(i_char);
	while (/*!ft_strncmp(res, argv[argc - 1], ft_strlen(res))
		||*/ !access(res, F_OK))
	{
		free(res);
		i_char = ft_itoa(i);
		res = ft_strjoin(tmp_file_name, i_char);
		if (!res)
			ft_exit(msh, 1);
		free(i_char);
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
	int		new_line;

	line = NULL;
	new_line = 1;
	ft_printf_fd(1, "> ");
	while (1)
	{
		free(line);
		line = get_next_line(0);
		if (!line && new_line)
		{
			ft_printf_fd(2, "\npipex: here-document delimited by end-of-file\n");
			break ;
		}
		if (line && new_line && !ft_strncmp(line, eof, ft_strlen(eof)))
			break ;
		new_line = contains_newline(line);
		if (new_line)
			ft_printf_fd(1, "> ");
		write(msh->in_fd, line, ft_strlen(line));
	}
	free(eof);
	free(line);
}

void	handle_here_doc(t_msh *msh, char *eof)
{
	char	*here_doc_file;

	here_doc_file = get_tmp_file_name(msh);
	msh->in_fd = open(here_doc_file, O_CREAT | O_RDWR, 0644);
	if (msh->in_fd == -1)
	{
		perror("open");
		ft_exit(msh, 1);
	}
	eof = ft_strjoin_free(eof, "\n", 1);
	if (!eof)
		ft_exit(msh, 1);
	get_here_doc_input(msh, eof);
	close(msh->in_fd);
	msh->in_fd = open(here_doc_file, O_RDWR, 0644);
	if (msh->in_fd == -1)
	{
		perror("open");
		ft_exit(msh, 1);
	}
}
