/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 18:29:20 by marde-vr          #+#    #+#             */
/*   Updated: 2024/04/24 14:41:01 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	export_invalid_identifier(char *arg, char *name)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	free(name);
	return (1);
}

t_env	*set_env(t_env *env, char *name, char *value, int append)
{
	t_env	*tmp;

	tmp = env;
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

t_env	*export_set_env(t_env *env, char *name, char *value, int append)
{
	if (!value || !name)
	{
		free(name);
		free(value);
		ft_printf_fd(2, "minishell: malloc failed");
		return (env);
	}
	return (set_env(env, name, value, append));
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
