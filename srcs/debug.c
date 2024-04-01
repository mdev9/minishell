/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 15:46:50 by tomoron           #+#    #+#             */
/*   Updated: 2024/04/01 20:07:08 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_parsed_token(t_token *token)
{
	while (token)
	{
		printf("[");
		if (token->type == ARG)
			printf("ARG : \"%s\"", token->value);
		else if (token->type == RED_O)
			printf("RED_O");
		else if (token->type == RED_O_APP)
			printf("RED_O_APP");
		else if (token->type == RED_I)
			printf("RED_I");
		else if (token->type == HERE_DOC)
			printf("HERE_DOC");
		printf("] ");
		token = token->next;
	}
	printf("\n");
}

void	print_parsed_cmd(t_cmd *cmd)
{
	while(cmd)
	{
		if(cmd->cmd_type == CMD)
			printf("[CMD : %s] ", cmd->value);
		if(cmd->cmd_type == PAREN)
			printf("[PAREN : %s] ", cmd->value);
		if(cmd->cmd_type == AND)
			printf("[AND] ");
		if(cmd->cmd_type == OR)
			printf("[OR] ");
		if(cmd->cmd_type == PIPE)
			printf("[PIPE] ");
		cmd = cmd->next;
	}
	printf("\n");
}
