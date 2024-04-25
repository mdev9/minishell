/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomoron <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 12:53:29 by tomoron           #+#    #+#             */
/*   Updated: 2024/04/25 10:35:48 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

int	filename_corresponds(char *wildcard, char *value)
{
	if (*value == '.' && *wildcard != '.')
		return (0);
	while (*wildcard && *value)
	{
		if (*wildcard == '*')
		{
			while(*wildcard == '*' && wildcard[1] == '*')
				wildcard++;
			if (!wildcard[1])
				return (1);
			while (*value)
				if (filename_corresponds(wildcard + 1, value++))
					return (1);
			return (0);
		}
		else if (*wildcard == *value)
		{
			wildcard++;
			value++;
		}
		else
			return (0);
	}
	return (!*wildcard && !*value);
}

t_token	*get_all_files(DIR *dir, char *wildcard, int is_var)
{
	struct dirent	*content;
	t_token			*res;

	res = 0;
	content = readdir(dir);
	while (content)
	{
		if (filename_corresponds(wildcard, content->d_name))
			res = token_add_back(res, ft_strdup(content->d_name), is_var);
		content = readdir(dir);
	}
	return (res);
}

t_token	*add_token_back(t_token *res, t_token *next)
{
	t_token	*start;

	if (!res)
		return (next);
	start = res;
	while (res->next)
		res = res->next;
	res->next = next;
	return (start);
}

t_token	*expand_wildcards(t_token *res, char *value, int is_var)
{
	DIR		*dir;
	char	*cwd;
	t_token	*new;

	if (!ft_strchr(value, '*'))
		return (token_add_back(res, value, is_var));
	cwd = getcwd(NULL, 100000);
	if (!cwd)
		return (token_add_back(res, value, is_var));
	dir = opendir(cwd);
	free(cwd);
	if (!dir)
		return (token_add_back(res, value, is_var));
	new = get_all_files(dir, value, is_var);
	closedir(dir);
	if (!new)
		return (token_add_back(res, value, is_var));
	free(value);
	sort_wildcards_token(new);
	res = add_token_back(res, new);
	return (res);
}
