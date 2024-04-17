/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_wildcard_list.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomoron <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 09:12:33 by tomoron           #+#    #+#             */
/*   Updated: 2024/04/17 10:42:39 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		wildcard_cmp(char *s1, char *s2, char *order)
{
	while(*s1 && *s2)
	{
		while(*s1 && !ft_isalnum(*s1))
			s1++;
		while(*s2 && !ft_isalnum(*s2))
			s2++;
		if(*s1 != *s2)
			return ((ft_strchr(order, *s2) - order) - (ft_strchr(order, *s1)\
					- order));
		s1++;
		s2++;
	}
	return ((ft_strchr(order, *s2) - order) - (ft_strchr(order, *s1) - order));
}

void	sort_wildcards_token(t_token *list)
{
	t_token *tmp;
	t_token *start;
	char 	*swap;
	
	tmp = list;
	start = list;
	while(tmp)
	{
		list = start;
		while(list->next)
		{
			if(wildcard_cmp(list->value, list->next->value, "zZyYxXwWvVuUtTsSrRqQpPoOnNmMlLkKjJiIhHgGfFeEdDcCbBaA9876543210") > 0)
			{
				swap = list->value;
				list->value = list->next->value;
				list->next->value = swap;
			}
			list = list->next;
		}
		tmp = tmp->next;
	}
}
