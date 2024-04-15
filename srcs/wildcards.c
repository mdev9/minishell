/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomoron <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 12:53:29 by tomoron           #+#    #+#             */
/*   Updated: 2024/04/15 18:54:15 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

int		filename_corresponds(char *wildcard, char *value)
{
	if(*value == '.' && *wildcard != '.')
		return(0);
	while(*wildcard && *value && (*wildcard == '*' || *wildcard == *value))
	{
		if(*wildcard == '*')
		{
			while(*wildcard && *value && *value != wildcard[1])
				value++;
			wildcard++;
		}
		while(*wildcard && *value && *value == *wildcard && *wildcard != '*')
		{
			wildcard++;
			value++;
		}
	}
	return(!*wildcard);
}

t_token	*expand_wildcards(t_token *res, char *value)
{
	DIR *dir;
	char *cwd;
	struct dirent *content;

	if(!ft_strchr(value, '*'))
		return(token_add_back(res, value));
	cwd = getcwd(NULL, 100000);
	if(!cwd)
		return(token_add_back(res, value));
	dir = opendir(cwd);
	free(cwd);
	if(!dir)
		return(token_add_back(res, value));
	content = readdir(dir);
	while(content)
	{
		if(filename_corresponds(value, content->d_name))
			res = token_add_back(res,ft_strdup(content->d_name));
		content = readdir(dir);
	}
	closedir(dir);
	if(!res)
		return(token_add_back(res, value));
	free(value);
	return(res);
}
