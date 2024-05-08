/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 09:06:51 by marde-vr          #+#    #+#             */
/*   Updated: 2024/05/08 12:31:54 by tomoron          ###   ########.fr       */
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

	while (*line)
	{
		if (*line == '$')
		{
			var_name = get_var_name(line + 1);
			if (!var_name)
				return ;
			line += ft_strlen(var_name);
			if (!*var_name)
				write(msh->in_fd, "$", 1);
			else if (!ft_strcmp(var_name, "?"))
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

void	remove_here_doc_file(t_msh *msh)
{
	if (msh->here_doc_filename)
	{
		unlink(msh->here_doc_filename);
		free(msh->here_doc_filename);
		msh->here_doc_filename = 0;
	}
}
