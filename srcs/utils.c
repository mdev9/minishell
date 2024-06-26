/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 18:19:26 by marde-vr          #+#    #+#             */
/*   Updated: 2024/05/07 17:07:16 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_exit(t_msh *msh, int exit_code)
{
	free_msh(msh);
	rl_clear_history();
	exit(exit_code);
}

int	check_var_name(char *name)
{
	if (ft_isdigit(*name) || !*name)
		return (0);
	while (*name)
	{
		if (!ft_isalnum(*name) && *name != '_')
			return (0);
		name++;
	}
	return (1);
}

int	add_return_code_to_str(char *res)
{
	char	*var;
	int		i;

	i = 0;
	var = ft_itoa(g_return_code);
	while (var && var[i])
	{
		res[i] = var[i];
		i++;
	}
	free(var);
	return (i);
}

int	file_access(t_msh *msh, int *found)
{
	int	fd;

	fd = open(msh->tokens->value, O_DIRECTORY);
	if (fd != -1)
	{
		close(fd);
		ft_printf_fd(2, "minishell: %s: Is a directory\n", msh->tokens->value);
		g_return_code = 126;
		return (0);
	}
	if (access(msh->tokens->value, X_OK) != -1)
		*found = 1;
	else
	{
		ft_printf_fd(2, "minishell: %s: ", msh->tokens->value);
		perror("");
		g_return_code = 127;
		if (access(msh->tokens->value, F_OK) != -1)
			g_return_code = 126;
		return (0);
	}
	return (1);
}

void	close_all_pipes(t_msh *msh)
{
	int	i;

	i = 0;
	while (msh->fds && msh->fds[i])
	{
		close_pipe_fds(msh, i);
		i++;
	}
}
