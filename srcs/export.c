/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 18:29:20 by marde-vr          #+#    #+#             */
/*   Updated: 2024/03/21 13:58:13 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
void print_env_declare(t_env *env)
{
	while(env)
	{
		if(strcmp(env->name, "_"))
			printf("declare -x %s=\"%s\"\n", env->name, env->value);
		env = env->next;
	}
}

int	ft_export(t_msh *msh)
{
	t_cmd	*cmd;

	cmd = msh->cmds;
	if(cmd && !cmd->next)
		print_env_declare(msh->env);	
	return (0);
}

int	ft_unset(t_msh *msh)
{
	t_cmd	*cmd;
	t_env	*tmp_env;
	t_env	*prev;

	cmd = msh->cmds;
	tmp_env = msh->env;
	prev = 0;
	if(cmd && cmd->next && cmd->next->type == ARG)
	{
		while(tmp_env)
		{
			if(!strcmp(cmd->next->token, tmp_env->name))
			{
				free(tmp_env->name);
				free(tmp_env->value);
				if(!prev)
					msh->env = tmp_env->next;
				else
					prev->next = tmp_env->next;
				free(tmp_env);
				return(0);
			}
			prev = tmp_env;
			tmp_env = tmp_env->next;	
		}
	}
	return (0);
}
