/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 21:02:54 by marde-vr          #+#    #+#             */
/*   Updated: 2024/02/16 21:51:28 by marde-vr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cd(t_cmd *args, t_env *env)
{
	char	*cwd;
	char	*new_wd;

	cwd = getcwd(0, 0);
	if (args->next->next && args->next->next->type == ARG)
	{
		ft_printf_fd(2, "minishell: cd: too many arguments\n");
		return (1);
	}
	if (!args->next || args->next->type != ARG)
		new_wd = ft_get_env(env, "HOME");
	else
	{
		new_wd = args->next->token;
		if (chdir(new_wd) == -1)
			perror("chdir");
	}
	ft_putstr(cwd);
	(void)args;
	return (0);
}
