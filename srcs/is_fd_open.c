/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_fd_open.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 17:31:53 by marde-vr          #+#    #+#             */
/*   Updated: 2024/04/18 20:48:54 by marde-vr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../srcs/minishell.h"

int	is_fd_open(int fd)
{
	if (fcntl(fd, F_GETFL) == -1)
	{
		fprintf(stderr, "%d:closed\n", fd);
		return (0);
	}
	fprintf(stderr, "%d:open\n", fd);
	return (1);
}
