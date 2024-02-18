/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishellrc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 17:40:16 by marde-vr          #+#    #+#             */
/*   Updated: 2024/02/18 17:10:16 by marde-vr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_minishellrc(t_env **env, t_alias **aliases)
{
	char	*home;
	char	*rc_path;
	int		fd;
	char	*line;
	t_cmd	*parsed_cmd;

	home = ft_get_env(*env, "HOME");
	rc_path = ft_strjoin(home, "/.minishellrc");
	if (access(rc_path, R_OK) != -1)
	{
		fd = open(rc_path, O_RDONLY);
		if (fd == -1)
		{
			free(env);
			perror("open");
			return ;
		}
		line = get_next_line(fd);
		while (line)
		{
			parsed_cmd = parse_command(line, *env);
			exec_command(parsed_cmd, env, aliases);
			free(parsed_cmd);
			free(line);
			line = get_next_line(fd);
		}
		close(fd);
	}
	free(rc_path);
}
