/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 10:58:36 by marde-vr          #+#    #+#             */
/*   Updated: 2024/05/03 14:25:03 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*dup_env(t_env *env)
{
	t_env	*res;

	res = 0;
	while (env)
	{
		res = env_add_back(res, env->name, env->value, 0);
		env = env->next;
	}
	return (res);
}

void	sort_env(t_env *env)
{
	t_env	*tmp;
	t_env	*start;
	char	*tmp_str;

	tmp = env;
	start = env;
	while (tmp)
	{
		env = start;
		while (env)
		{
			if (ft_strcmp(tmp->name, env->name) < 0)
			{
				tmp_str = tmp->name;
				tmp->name = env->name;
				env->name = tmp_str;
				tmp_str = tmp->value;
				tmp->value = env->value;
				env->value = tmp_str;
			}
			env = env->next;
		}
		tmp = tmp->next;
	}
}

void	print_env_declare(t_msh *msh, t_env *env_orig)
{
	t_env	*env;
	t_env	*start;

	env = dup_env(env_orig);
	sort_env(env);
	start = env;
	if (!msh->out_fd)
		msh->out_fd = 1;
	while (env)
	{
		if (strcmp(env->name, "_"))
		{
			if (env->value)
				ft_printf_fd(msh->out_fd, "declare -x %s=\"%s\"\n", env->name,
					env->value);
			else
				ft_printf_fd(msh->out_fd, "declare -x %s\n", env->name);
		}
		env = env->next;
	}
	free_env_cpy(start);
}

void	delete_from_env(t_msh *msh, char *name)
{
	t_env	*tmp_env;
	t_env	*prev;

	tmp_env = msh->env;
	prev = 0;
	while (tmp_env)
	{
		if (!strcmp(name, tmp_env->name))
		{
			free(tmp_env->name);
			free(tmp_env->value);
			if (!prev)
				msh->env = tmp_env->next;
			else
				prev->next = tmp_env->next;
			free(tmp_env);
			return ;
		}
		prev = tmp_env;
		tmp_env = tmp_env->next;
	}
}
