/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomoron <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 17:19:27 by tomoron           #+#    #+#             */
/*   Updated: 2024/03/28 13:55:48 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*parsing_syntax_error(t_token *res)
{
	free_token(res);
	ft_putstr_fd("minishell: syntax error\n", 2);
	return (0);
}
