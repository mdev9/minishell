/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 18:29:20 by marde-vr          #+#    #+#             */
/*   Updated: 2024/04/23 12:49:46 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*dup_env(t_env *env)
{
	t_env *res;

	res = 0;
	while(env)
	{
		res = env_add_back(res, env->name, env->value);
		env = env->next;
	}
	return(res);
}

void	sort_env(t_env *env)
{
	t_env *tmp;
	t_env *start;
	char *tmp_str;

	tmp = env;
	start = env;
	while(tmp)
	{
		env = start;
		while(env)
		{
			if(ft_strcmp(tmp->name, env->name) < 0)
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
	t_env *env;

	env = dup_env(env_orig);
	sort_env(env);
	if(!msh->out_fd)
		msh->out_fd = 1;
	while (env)
	{
		if (strcmp(env->name, "_"))
		{
			if (env->value && *(env->value))
				ft_printf_fd(msh->out_fd, "declare -x %s=\"%s\"\n", env->name, env->value);
			else
				ft_printf_fd(msh->out_fd, "declare -x %s\n", env->name);
		}
		env = env->next;
	}
}

int	export_invalid_identifier(char *arg, char *name)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	free(name);
	return (1);
}

t_env	*export_set_env(t_env *env, char *name, char *value, int append)
{
	t_env	*tmp;

	tmp = env;
	if (!value)
	{
		free(name);
		ft_printf_fd(2, "minishell: malloc failed");
		return (env);
	}
	while (tmp)
	{
		if (!ft_strcmp(name, tmp->name))
		{
			free(name);
			if (!*value)
			{
				free(value);
				return (env);
			}
			if (append)
				value = ft_strjoin_free(tmp->value, value, 2);
			if (!value)
				return (env);
			free(tmp->value);
			tmp->value = value;
			return (env);
		}
		tmp = tmp->next;
	}
	return (env_add_back(env, name, value));
}

int	ft_export(t_msh *msh, t_token *cmd, t_env *env)
{
	char	*arg;
	char	*name;
	char	*value;
	int		len;
	int		append;

	if (cmd && !cmd->next)
		print_env_declare(msh, env);
	if (cmd && cmd->next && !cmd->next->next)
	{
		arg = cmd->next->value;
		len = 0;
		while (arg[len] && arg[len] != '=' && arg[len] != '+')
			len++;
		name = ft_substr(arg, 0, len);
		append = arg[len] == '+';
		if (arg[len] == '=' || (arg[len] == '+' && arg[len + 1] == '='))
			len += 1 + (arg[len] == '+');
		if (!name || !check_var_name(name) || arg[len] == '+')
			return (export_invalid_identifier(arg, name));
		value = ft_strdup(arg + len);
		env = export_set_env(env, name, value, append);
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
	t_token	*cmd;

	cmd = msh->tokens;
	if (cmd)
		cmd = cmd->next;
	while (cmd)
	{
		delete_from_env(msh, cmd->value);
		cmd = cmd->next;
	}
	return (0);
}
