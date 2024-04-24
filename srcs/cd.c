/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 21:02:54 by marde-vr          #+#    #+#             */
/*   Updated: 2024/04/24 10:43:52 by marde-vr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cd_update_pwd(t_msh *msh)
{
	char	*pwd;
	char	*new;

	new = getcwd(0, 16382);
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

char	*get_new_wd(t_token *arg, t_msh *msh)
{
	char	*nw_wd;

	if (arg)
	{
		nw_wd = arg->value;
		if (!ft_strcmp("-", nw_wd))
		{
			nw_wd = ft_get_env(msh->env, "OLDPWD");
			if (!nw_wd)
				ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
			if (!nw_wd)
				return (0);
			ft_printf_fd((1 * (msh->out_fd == 0)) + msh->out_fd, "%s\n", nw_wd);
		}
	}
	else
	{
		nw_wd = ft_get_env(msh->env, "HOME");
		if (!nw_wd)
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
		if (!nw_wd)
			return (0);
	}
	return (nw_wd);
}

int	cd(t_token *args, t_msh *msh)
{
	char	*new_wd;

	if (args->next && args->next->next)
	{
		ft_printf_fd(2, "minishell: cd: too many arguments\n");
		return (1);
	}
	new_wd = get_new_wd(args->next, msh);
	if (!new_wd)
		return (1);
	if (chdir(new_wd) == -1)
	{
		perror("minishell: cd");
		return (1);
	}
	cd_update_pwd(msh);
	return (0);
}
