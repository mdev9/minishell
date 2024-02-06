/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomoron <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 21:59:20 by tomoron           #+#    #+#             */
/*   Updated: 2024/02/06 22:21:44 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int g_return_code = 0;

char *get_prompt(void)
{
	char *res;

	res = ft_strjoin_free("\001",ft_get_color(0,255,0),2);
	res = ft_strjoin_free(res,"\002", 1);
	res = ft_strjoin_free(res, getenv("USER"),1);
	res = ft_strjoin_free(res, "@",1);
	res = ft_strjoin_free(res, "minishell \001\033[0m\002$>",1);
	return(res);
}

int ft_get_token_len(char *command)
{
	int in_quote;
	int in_dquote;
	int res;

	in_quote = 0;
	in_dquote = 0;
	res = 0;
	while(*command && (!isspace(*command) || in_quote || in_dquote))
	{
		if(*command == '"' && !in_quote)
			in_dquote = !in_dquote;
		if(*command == '\'' && !in_dquote)
			in_quote = !in_quote;
		if(*command != '\'' && *command != '"')
			res++;
		if((*command == '\'' && in_dquote) || (*command == '"' && in_quote))
			res++;
		command++;
	}
	return(res);
}

void ft_skip_spaces(char **command)
{
	while (ft_isspace(**command))
		(*command)++;
}

char *ft_get_token(char **command)
{
	int in_quote;
	int in_dquote;
	char *res;
	int	i;

	in_quote = 0;
	in_dquote = 0;
	i = 0;
	while (ft_isspace(**command))
		(*command)++;
	res = malloc(ft_get_token_len(*command));
	while(res && **command && (!isspace(**command) || in_quote || in_dquote))
	{
		if(**command == '"' && !in_quote)
			in_dquote = !in_dquote;
		if(**command == '\'' && !in_dquote)
			in_quote = !in_quote;
		if(((**command == '\'' && in_dquote) || (**command == '"' && in_quote))
			|| (**command != '\'' && **command != '"'))
				res[i++] = **command;
		(*command)++;
	}
	return(res);
}

t_cmd *ft_parse_command(char *command)
{	
	t_cmd *res;
	char *token;

	res = 0;
	while(*command)
	{
		token = ft_get_token(&command);
		res = ft_cmd_add_back(res, token);
	}
	return(res);
}

int main(void)
{
	char *command;
	char *prompt;

	command = (char *)1;
	while(command)
	{
		prompt = get_prompt();
		if(!prompt)
			exit(0);
		command = readline(prompt);
		add_history(command);
		free(prompt);
		t_cmd *test;
		test = ft_parse_command(command);
		printf("%s\n",test->token);
		printf("%s\n",command);
		if(command && !ft_strcmp(command,"exit"))
		{
			rl_clear_history();
			free(command);
			printf("au revoir :,(\n");
			exit(0);
		}
		rl_clear_history();
		free(command);
	}
}
