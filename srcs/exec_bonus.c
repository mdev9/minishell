/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomoron <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 13:50:14 by tomoron           #+#    #+#             */
/*   Updated: 2024/03/29 14:47:40 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_command_bonus(t_msh *msh, char *cmd_str)
{
	t_cmd *cmd;

	(void)msh;
	printf("cmd : %s\n",cmd_str);
	cmd = parsing_bonus(cmd_str);
	printf("%p\n", cmd);
	print_parsed_cmd(cmd);
}
