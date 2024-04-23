/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 21:02:54 by marde-vr          #+#    #+#             */
/*   Updated: 2024/04/23 18:34:12 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cd_update_pwd(char *new_path, t_msh *msh)
{
	char	*pwd;
	char	*new;

	new = ft_strdup(new_path);
	if (!new)
		return ;
	pwd = ft_get_env(msh->env, "PWD");
	pwd = ft_strdup(pwd);
	if (!pwd)
		ft_strdup("");
	if (!pwd)
		free(new);
	if (!pwd)
		return ;
	msh->env = export_set_env(msh->env, ft_strdup("OLDPWD"), pwd, 0);
	if (ft_get_env(msh->env, "PWD"))
		msh->env = export_set_env(msh->env, ft_strdup("PWD"), new, 0);
}

int	cd(t_token *args, t_env *env, t_msh *msh)
{
	char	*new_wd;

	if (args->next && args->next->next)
	{
		ft_printf_fd(2, "minishell: cd: too many arguments\n");
		return (1);
	}
	if (!args->next)
	{
		new_wd = ft_get_env(env, "HOME");
		if (!new_wd)
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
		if (!new_wd)
			return (1);
	}
	else
		new_wd = args->next->value;
	cd_update_pwd(new_wd, msh);
	if (chdir(new_wd) == -1)
	{
		perror("minishell: cd");
		return (1);
	}
	return (0);
}
