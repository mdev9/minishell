/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishellrc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 17:40:16 by marde-vr          #+#    #+#             */
/*   Updated: 2024/02/21 17:34:26 by marde-vr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_rc_file(t_msh *msh, int fd)
{
	char	*line;

	line = get_next_line(fd);
	while (line)
	{
		if (line[0] != '#')
		{
			msh->cmds = parse_command(line, msh->env);
			exec_command(msh);
			free_cmd(msh->cmds);
		}
		free(line);
		line = get_next_line(fd);
	}
}

void	handle_minishellrc(t_msh *msh)
{
	char	*home;
	char	*rc_path;
	int		fd;

	home = ft_get_env(msh->env, "HOME");
	rc_path = ft_strjoin(home, "/.minishellrc");
	if (access(rc_path, R_OK) != -1)
	{
		fd = open(rc_path, O_RDONLY);
		if (fd == -1)
		{
			free(msh->env);
			perror("open");
			return ;
		}
		exec_rc_file(msh, fd);
		close(fd);
	}
	free(rc_path);
}
