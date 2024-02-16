/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 21:02:54 by marde-vr          #+#    #+#             */
/*   Updated: 2024/02/16 22:06:55 by marde-vr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cd(t_cmd *args)
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
		new_wd = getenv("HOME");
	else
	{
		new_wd = args->next->token;
		if (chdir(new_wd) == -1)
			perror("minishell: cd");
	}
	return (0);
}
