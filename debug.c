/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomoron <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 14:16:47 by tomoron           #+#    #+#             */
/*   Updated: 2024/02/15 14:45:29 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_parsed_cmd(t_cmd *cmd)
{
	while(cmd)
	{
		printf("[");
		if(cmd->type == ARG)
			printf("ARG : %s",cmd->token);
		else if(cmd->type == PIPE)
			printf("PIPE");
		else if(cmd->type == OR)
			printf("OR");
		else if(cmd->type == AND)
			printf("AND");
		printf("] ");
		cmd = cmd->next;
	}
	printf("\n");
}
