/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_fd_open.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 17:31:53 by marde-vr          #+#    #+#             */
/*   Updated: 2024/03/05 17:35:39 by marde-vr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../srcs/minishell.h"

int	is_fd_open(int fd)
{
	if (fcntl(fd, F_GETFL) == -1)
	{
		ft_printf_fd(2, "%d:closed\n", fd);
		return (0);
	}
	ft_printf_fd(2, "%d:open\n", fd);
	return (1);
}
