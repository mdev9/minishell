/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 20:46:19 by tomoron           #+#    #+#             */
/*   Updated: 2024/04/29 22:11:22 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*env_add_back(t_env *env, char *name, char *value)
{
	t_env	*res;
	t_env	*current;

	res = ft_calloc(1, sizeof(t_env));
	if (!res)
		return (env);
	res->name = name;
	res->value = value;
	if (!env)
		return (res);
	current = env;
	while (current->next)
		current = current->next;
	current->next = res;
	return (env);
}

int	print_env(t_env *env)
{
	while (env)
	{
		if (*env->value)
			ft_printf("%s=%s\n", env->name, env->value);
		env = env->next;
	}
	return (0);
}

char	*ft_get_env(t_env *env, char *name)
{
	while (env)
	{
		if (!ft_strcmp(env->name, name))
			return (env->value);
		env = env->next;
	}
	return (0);
}

int	get_var_name_len(char *command)
{
	int	res;

	res = 0;
	if (command[res] == '?')
		return (1);
	while (ft_isalnum(command[res]) || command[res] == '_')
		res++;
	return (res);
}
