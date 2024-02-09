/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomoron <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 21:59:20 by tomoron           #+#    #+#             */
/*   Updated: 2024/02/09 14:37:48 by tomoron          ###   ########.fr       */
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
	return (res);
}

char *ft_getenv(t_env *env, char *name)
{
	while(env)
	{
		if(!ft_strcmp(env->name, name))
			return(env->value);
		env = env->next;
	}
	return(0);
}

int get_var_name_len(char *command)
{
	int res;

	res = 0;
	if(command[res] == '?')
		return(1);
	while(ft_isalnum(command[res]) || command[res] == '_')
		res++;
	return(res);
}

char *get_var_name(char *command)
{
	char *res;
	int len;

	len = get_var_name_len(command);
	res = ft_substr(command,0, len);
	return(res);
}

int	ft_get_var_len(char **command, t_env *env)
{
	char *var_name;
	char *env_var;

	(*command)++;
	if (!ft_isalnum(**command) && **command != '_' && **command != '?')
		return (1);
	if (**command == '?')
		return (get_number_len(g_return_code));
	var_name = get_var_name(*command);
	env_var = ft_getenv(env, var_name);
	free(var_name);
	if (!env_var)
		return (0);
	*command += get_var_name_len(*command) - 1;
	return (ft_strlen(env_var));
}

int ft_get_token_len(char *command, t_env *env)
{
	int in_quote;
	int in_dquote;
	int res;

	in_quote = 0;
	in_dquote = 0;
	res = 0;
	while (*command && (!ft_isspace(*command) || in_quote || in_dquote))
	{
		if (*command == '"' && !in_quote)
			in_dquote = !in_dquote;
		if (*command == '\'' && !in_dquote)
			in_quote = !in_quote;
		if (*command == '$' && !in_quote)
			res += ft_get_var_len(&command, env);
		else if (*command != '\'' && *command != '"')
			res++;
		else if ((*command == '\'' && in_dquote) || (*command == '"' && in_quote))
			res++;
		command++;
	}
	return (res);
}

int	ft_add_var_to_str(char *res, char **command, t_env *env)
{
	char *var_name;
	char *var;
	int i;

	i = -1;
	if (!ft_isalnum(**command) && **command != '_' && **command != '?')
	{
		*res = '$';
		return(1);
	}
	if (**command == '?')
	{
		var = ft_itoa(g_return_code);
		while(var && var[++i])
			res[i] = var[i];
		free(var);
		return(i + 1);
	}
	var_name = get_var_name(*command);
	var = ft_getenv(env, var_name);
	free(var_name);
	while(var && var[++i])
		res[i] = var[i];
	*command += get_var_name_len(*command) - 1;
	return (i + !var);
}

char *ft_get_token(char **command, int *in_quote, int *in_dquote, t_env *env)
{
	char *res;
	int	i;

	i = 0;
	while (ft_isspace(**command))
		(*command)++;
	res = ft_calloc(ft_get_token_len(*command, env) + 1, 1);
	while (res && **command && (!ft_isspace(**command) || *in_quote || *in_dquote))
	{
		if (**command == '"' && !*in_quote)
			*in_dquote = !*in_dquote;
		if (**command == '\'' && !*in_dquote)
			*in_quote = !*in_quote;
		if (**command == '$' && !*in_quote)
		{
			(*command)++;
			i += ft_add_var_to_str(res + i, command , env);
		}
		else if (((**command == '\'' && *in_dquote) || (**command == '"' && *in_quote))
			|| (**command != '\'' && **command != '"'))
				res[i++] = **command;
		(*command)++;
	}
	return (res);
}

t_cmd *ft_parse_command(char *command, t_env *env)
{	
	int in_quote;
	int in_dquote;
	t_cmd *res;
	char *token;

	in_quote = 0;
	in_dquote = 0;
	res = STDIN_FILENO;
	if (!command)
		return (STDIN_FILENO);
	while (*command)
	{
		token = ft_get_token(&command, &in_quote, &in_dquote, env);
		res = ft_cmd_add_back(res, token);
	}
	if (in_quote || in_dquote)
	{
		ft_free_cmd(res);
		ft_putstr_fd("minishell: syntax error\n", 2);
		return (0);
	}
	return (res);
}

t_env	*ft_get_env(char **envp)
{
	t_env *env;
	char *name;
	char *value;
	int	i;
	int	j;

	env = 0;
	while (*envp)
	{
		i = 0;
		j = 0;
		while ((*envp)[i] && (*envp)[i] != '=')
			i++;
		while ((*envp)[i + 1 + j])
			j++;
		name = ft_substr(*envp, 0, i);
		value = ft_substr(*envp, i + 1, j);
		env = ft_env_add_back(env, name, value);
		if (!name || !value)
			ft_free_env(env);
		if (!name || !value)
			return (0);
		envp++;
	}
	return (env);
}

int main(int argc, char **argv, char **envp)
{
	char *command;
	t_cmd *parsed_cmd;
	t_env *env;
	char *prompt;

	command = (char *)STDOUT_FILENO;
	(void)argc;
	(void)argv;
	env = ft_get_env(envp);
	while (env && command)
	{
		prompt = get_prompt();
		if (!prompt)
			exit(STDIN_FILENO);
		command = readline(prompt);
		free(prompt);
		add_history(command);
		parsed_cmd = ft_parse_command(command, env);
		free(command);
		ft_exec_command(parsed_cmd, env);
		ft_free_cmd(parsed_cmd);
	}
	rl_clear_history();
	ft_free_env(env);
	return (g_return_code);
}
