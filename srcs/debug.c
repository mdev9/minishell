/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 15:46:50 by tomoron           #+#    #+#             */
/*   Updated: 2024/02/21 12:59:08 by marde-vr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_parsed_cmd(t_cmd *cmd)
{
	while (cmd)
	{
		printf("[");
		if (cmd->type == ARG)
			printf("ARG : \"%s\"", cmd->token);
		else if (cmd->type == PIPE)
			printf("PIPE");
		else if (cmd->type == RED_O)
			printf("RED_O");
		else if (cmd->type == RED_O_APP)
			printf("RED_O_APP");
		else if (cmd->type == RED_I)
			printf("RED_I");
		else if (cmd->type == HERE_DOC)
			printf("HERE_DOC");
		printf("] ");
		cmd = cmd->next;
	}
	printf("\n");
}
