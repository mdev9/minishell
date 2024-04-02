/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 15:46:50 by tomoron           #+#    #+#             */
/*   Updated: 2024/04/02 00:45:25 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_parsed_token(t_token *token)
{
	while (token)
	{
		printf("[");
		printf("ARG : \"%s\"", token->value);
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
