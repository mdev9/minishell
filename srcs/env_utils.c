/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 13:30:18 by tomoron           #+#    #+#             */
/*   Updated: 2024/04/18 20:48:52 by marde-vr         ###   ########.fr       */
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

char	**split_paths_from_env(t_env *env)
{
	t_env	*cur_env_var;
	int		path_in_envp;

	path_in_envp = 0;
	cur_env_var = env;
	while (cur_env_var && cur_env_var->next != 0)
	{
		if (!ft_strcmp(cur_env_var->name, "PATH"))
		{
			path_in_envp = 1;
			break ;
		}
		cur_env_var = cur_env_var->next;
	}
	if (!path_in_envp)
		return (0);
	return (ft_split(cur_env_var->value, ':'));
}
