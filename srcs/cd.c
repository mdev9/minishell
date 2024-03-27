/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 21:02:54 by marde-vr          #+#    #+#             */
/*   Updated: 2024/03/27 16:21:48 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cd(t_token *args)
{
	char	*new_wd;

	if (args->next && args->next->next && args->next->next->type == ARG)
	{
		ft_printf_fd(2, "minishell: cd: too many arguments\n");
		g_return_code = 1;
		return (1);
	}
	if (!args->next || args->next->type != ARG)
		new_wd = getenv("HOME");
	else
		new_wd = args->next->value;
	if (chdir(new_wd) == -1)
	{
		perror("minishell: cd");
		g_return_code = 1;
	}
	return (0);
}
