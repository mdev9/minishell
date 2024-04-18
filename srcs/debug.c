/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 15:46:50 by tomoron           #+#    #+#             */
/*   Updated: 2024/04/18 20:48:51 by marde-vr         ###   ########.fr       */
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

void	print_cmd_type(t_cmd_type type, char *value)
{
	if (type == CMD)
		printf("[CMD : %s] ", value);
	if (type == PAREN)
		printf("[PAREN : %s] ", value);
	if (type == AND)
		printf("[AND] ");
	if (type == OR)
		printf("[OR] ");
	if (type == PIPE)
		printf("[PIPE] ");
	if (type == RED_O_APP)
		printf("[RED_O_APP : %s] ", value);
	if (type == RED_O)
		printf("[RED_O : %s] ", value);
	if (type == RED_I)
		printf("[RED_I : %s] ", value);
	if (type == HERE_DOC)
		printf("[HERE_DOC : %s] ", value);
}

void	print_parsed_cmd(t_cmd *cmd)
{
	while (cmd)
	{
		print_cmd_type(cmd->cmd_type, cmd->value);
		cmd = cmd->next;
	}
	printf("\n");
}

void	print_msh_struct(t_msh *msh)
{
	printf("in_fd : %d\n", msh->in_fd);
	printf("out_fd : %d\n", msh->out_fd);
	printf("in_type: ");
	print_cmd_type(msh->in_type, 0);
	printf("\nout_type: ");
	print_cmd_type(msh->out_type, 0);
	printf("\n");
}
