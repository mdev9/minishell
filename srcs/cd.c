/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 21:02:54 by marde-vr          #+#    #+#             */
/*   Updated: 2024/04/18 17:56:04 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cd(t_token *args)
{
	char	*new_wd;

	if (args->next && args->next->next)
	{
		fprintf(stderr, "minishell: cd: too many arguments\n");
		g_return_code = 1;
		return (1);
	}
	if (!args->next)
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
