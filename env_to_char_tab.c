/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_to_char_tab.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 13:30:18 by tomoron           #+#    #+#             */
/*   Updated: 2024/02/16 15:08:53 by marde-vr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_env_len(t_env *env)
{
	int	res;

	res = 0;
	while (env)
	{
		res++;
		env = env->next;
	}
	return (res);
}

char	**env_to_char_tab(t_env *env)
{
	char	**res;
	int		i;
	char	*tmp;

	res = ft_calloc(get_env_len(env) + 1, sizeof(char *));
	i = 0;
	while (res && env)
	{
		tmp = ft_strjoin(env->name, "=");
		tmp = ft_strjoin_free(tmp, env->value, 1);
		res[i] = tmp;
		i++;
		env = env->next;
	}
	return (res);
}
