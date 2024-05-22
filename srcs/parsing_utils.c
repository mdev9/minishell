/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomoron <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 13:49:17 by tomoron           #+#    #+#             */
/*   Updated: 2024/05/22 13:50:31 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_home_var_len(void)
{
	char	*str;

	str = getenv("HOME");
	if (!str)
		return (1);
	else
		return (ft_strlen("HOME"));
}
