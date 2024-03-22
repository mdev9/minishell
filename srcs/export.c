/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 18:29:20 by marde-vr          #+#    #+#             */
/*   Updated: 2024/03/22 13:52:29 by tomoron          ###   ########.fr       */
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

int	check_var_name(char *name)
{
	while(*name)
	{
		if(!ft_isalnum(*name) && *name != '_')
			return(0);
		name++;
	}
	return(1);
}

int	ft_export(t_msh *msh)
{
	t_cmd	*cmd;
	char 	*arg;
	char	*name;
	char	*value;
	int		len;

	cmd = msh->cmds;
	if(cmd && (!cmd->next || (cmd->next && cmd->next->type != ARG)))
		print_env_declare(msh->env);	
	if(cmd && cmd->next && cmd->next->type == ARG && (!cmd->next->next ||
		(cmd->next->next && cmd->next->next->type != ARG)))
	{
		arg = cmd->next->token;
		len = 0;
		while(arg[len] && arg[len] != '=')
			len++;
		name = ft_substr(arg, 0, len);
		if(!name || !check_var_name(name))
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(name, 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			free(name);
			return(1);
		}
		if(arg[len])
			len++;
		value = ft_strdup(arg + len);
		msh->env = env_add_back(msh->env, name, value);
	}
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
