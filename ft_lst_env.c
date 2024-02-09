/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lst_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomoron <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 20:46:19 by tomoron           #+#    #+#             */
/*   Updated: 2024/02/08 16:58:48 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env *ft_env_add_back(t_env *env, char *name, char *value)
{
	t_env *res;
	t_env *current;

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

void	ft_free_env(t_env *env)
{
	if(env && env->next)
		ft_free_env(env->next);
	if(env)
	{
		free(env->name);
		free(env->value);
	}
	free(env);
}

int	ft_print_env(t_env *env)
{
	while(env)
	{
		printf("%s=%s\n",env->name, env->value);
		env = env->next;
	}
	return(0);
}
