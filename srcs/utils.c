/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 18:19:26 by marde-vr          #+#    #+#             */
/*   Updated: 2024/03/28 13:56:58 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_msh(t_msh *msh)
{
	if (msh)
	{
		free_env(msh->env);
		free(msh->pids);
		free(msh->fds);
		free_token(msh->cmds);
		set_echoctl(msh->echoctl);
		free(msh);
	}
}

void	ft_exit(t_msh *msh, int exit_code)
{
	free_msh(msh);
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

	fd = open(msh->cmds->value, O_DIRECTORY);
	if (fd != -1)
	{
		close(fd);
		ft_printf_fd(2, "minishell: %s: Is a directory\n", msh->cmds->value);
		g_return_code = 126;
		return (0);
	}
	if (access(msh->cmds->value, X_OK) != -1)
		*found = 1;
	else
	{
		ft_printf_fd(2, "minishell: %s: ", msh->cmds->value);
		perror("");
		g_return_code = 127;
		if (access(msh->cmds->value, F_OK) != -1)
			g_return_code = 126;
		return (0);
	}
	return (1);
}
