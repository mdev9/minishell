/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 14:11:22 by marde-vr          #+#    #+#             */
/*   Updated: 2024/05/03 14:11:50 by marde-vr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ambiguous_redirect(char *str, t_msh *msh)
{
	ft_printf_fd(2, "minishell: %s: ambiguous redirect\n", str);
	msh->in_fd = -2;
	g_return_code = 1;
}

int	first_is_in_type(t_cmd *cmd)
{
	t_cmd	*cur_token;

	cur_token = cmd;
	while (cur_token && cur_token->cmd_type == CMD && cur_token->next)
		cur_token = cur_token->next;
	if (is_input_type(cur_token) || cur_token->cmd_type == PIPE)
		return (1);
	return (0);
}
