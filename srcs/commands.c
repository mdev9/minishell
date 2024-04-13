/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 18:22:15 by marde-vr          #+#    #+#             */
/*   Updated: 2024/04/07 18:40:53 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_args_count(t_token *cmds)
{
	int		count;
	t_token	*cur_cmd;

	count = 0;
	cur_cmd = cmds;
	if (cur_cmd)
		count++;
	while (cur_cmd->next)
	{
		cur_cmd = cur_cmd->next;
		count++;
	}
	return (count);
}

char	**get_cmd_args(t_msh *msh)
{
	char	**cmd_args;
	t_token	*cur_cmd;
	int		args_count;
	int		i;

	args_count = get_args_count(msh->tokens);
	cmd_args = ft_calloc(args_count + 1, sizeof(char *));
	if (!cmd_args)
		ft_exit(msh, 1);
	cur_cmd = msh->tokens;
	i = 0;
	while (i < args_count)
	{
		if (cur_cmd)
		{
			if (!i)
				cmd_args[i++] = remove_path(cur_cmd->value);
			else
				cmd_args[i++] = cur_cmd->value;
		}
		else
			cur_cmd = cur_cmd->next;
		cur_cmd = cur_cmd->next;
	}
	return (cmd_args);
}

void	remove_command_from_msh(t_msh *msh)
{
	free_token(msh->tokens);
	while(msh->cmds && is_cmd_type(msh->cmds))
		msh->cmds = msh->cmds->next;
	while(msh->cmds && !is_cmd_type(msh->cmds))
		msh->cmds = msh->cmds->next;
	if(msh->cmds)
		msh->tokens = parse_command(msh->cmds->value, msh->env);
	else
		msh->tokens = 0;
}
