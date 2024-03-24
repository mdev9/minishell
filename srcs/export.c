/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 18:29:20 by marde-vr          #+#    #+#             */
/*   Updated: 2024/03/24 08:49:17 by marde-vr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_env_declare(t_env *env)
{
	while (env)
	{
		if (strcmp(env->name, "_"))
			printf("declare -x %s=\"%s\"\n", env->name, env->value);
		env = env->next;
	}
}

int	check_var_name(char *name)
{
	if (ft_isdigit(*name) || !*name)
		return (0);
	while (*name)
	{
		if (!ft_isalnum(*name) && *name != '_')
			return (0);
		name++;
	}
	return (1);
}

int	ft_export(t_msh *msh)
{
	t_cmd	*cmd;
	char	*arg;
	char	*name;
	char	*value;
	int		len;

	cmd = msh->cmds;
	if (cmd && (!cmd->next || (cmd->next && cmd->next->type != ARG)))
		print_env_declare(msh->env);
	if (cmd && cmd->next && cmd->next->type == ARG && (!cmd->next->next
			|| (cmd->next->next && cmd->next->next->type != ARG)))
	{
		arg = cmd->next->token;
		len = 0;
		while (arg[len] && arg[len] != '=')
			len++;
		name = ft_substr(arg, 0, len);
		if (!name || !check_var_name(name))
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(arg, 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			free(name);
			return (1);
		}
		if (arg[len])
			len++;
		value = ft_strdup(arg + len);
		msh->env = env_add_back(msh->env, name, value);
	}
	return (0);
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

int	ft_unset(t_msh *msh)
{
	t_cmd	*cmd;

	cmd = msh->cmds;
	if (cmd)
		cmd = cmd->next;
	while (cmd && cmd->type == ARG)
	{
		delete_from_env(msh, cmd->token);
		cmd = cmd->next;
	}
	return (0);
}
