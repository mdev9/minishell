/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 18:31:21 by tomoron           #+#    #+#             */
/*   Updated: 2024/02/16 21:19:30 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

int	pwd(void)
{
	char	*buffer;

	buffer = getcwd(NULL, 0);
	if(!buffer)
		return(1);
	ft_printf("%s\n", buffer);
	free(buffer);
	return (0);
}
