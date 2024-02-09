/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomoron <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 16:04:11 by tomoron           #+#    #+#             */
/*   Updated: 2024/02/07 16:48:07 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_exit(t_cmd *args)
{
	t_cmd *start;	

	start = args;
	args = args->next;
	ft_printf("exit\n");
	if(args && args->next)
		ft_printf("minishell: exit: too many arguments\n");
	else
	{
		ft_free_cmd(start);
		if(args)
			exit((unsigned char)ft_atoi(args->token));
		exit(g_return_code);
	}
}
