/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 15:46:50 by tomoron           #+#    #+#             */
/*   Updated: 2024/04/02 17:22:43 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_parsed_token(t_token *token)
{
	while (token)
	{
		printf("[ARG : \"%s\"]", token->value);
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
		if(cmd->cmd_type == RED_O_APP)
			printf("[RED_O_APP : %s] ", cmd->value);
		if(cmd->cmd_type == RED_O)
			printf("[RED_O : %s] ", cmd->value);
		if(cmd->cmd_type == RED_I)
			printf("[RED_I : %s] ", cmd->value);
		if(cmd->cmd_type == HERE_DOC)
			printf("[HERE_DOC : %s] ", cmd->value);
		cmd = cmd->next;
	}
	printf("\n");
}

void	print_msh_struct(t_msh *msh)
{
	printf("in_fd : %d\n", msh->in_fd);
	printf("out_fd : %d\n", msh->out_fd);
}
