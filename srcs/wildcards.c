/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomoron <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 12:53:29 by tomoron           #+#    #+#             */
/*   Updated: 2024/04/18 20:48:39 by marde-vr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

int	filename_corresponds(char *wildcard, char *value)
{
	if (*value == '.' && *wildcard != '.')
		return (0);
	while (*wildcard && (*wildcard == '*' || *wildcard == *value))
	{
		if (*wildcard == '*')
		{
			while (*wildcard && *value && *value != wildcard[1]
				&& *wildcard != wildcard[1])
				value++;
			wildcard++;
		}
		while (*wildcard && *value && *value == *wildcard && *wildcard != '*')
		{
			wildcard++;
			value++;
		}
	}
	return (!*wildcard && !*value);
}

t_token	*get_all_files(DIR *dir, char *wildcard)
{
	struct dirent	*content;
	t_token			*res;

	res = 0;
	content = readdir(dir);
	while (content)
	{
		if (filename_corresponds(wildcard, content->d_name))
			res = token_add_back(res, ft_strdup(content->d_name));
		content = readdir(dir);
	}
	return (res);
}

t_token	*wildcards_add_back(t_token *res, t_token *next)
{
	if (!res)
		return (next);
	while (res->next)
		res = res->next;
	res->next = next;
	return (res);
}

t_token	*expand_wildcards(t_token *res, char *value)
{
	DIR		*dir;
	char	*cwd;
	t_token	*new;

	if (!ft_strchr(value, '*'))
		return (token_add_back(res, value));
	cwd = getcwd(NULL, 100000);
	if (!cwd)
		return (token_add_back(res, value));
	dir = opendir(cwd);
	free(cwd);
	if (!dir)
		return (token_add_back(res, value));
	new = get_all_files(dir, value);
	closedir(dir);
	if (!new)
		return (token_add_back(res, value));
	free(value);
	sort_wildcards_token(new);
	res = wildcards_add_back(res, new);
	return (res);
}
