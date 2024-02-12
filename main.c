/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomoron <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 21:59:20 by tomoron           #+#    #+#             */
/*   Updated: 2024/02/12 14:53:26 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_return_code = 0;

char	*get_prompt(void)
{
	char	*res;
	char	cwd_buffer[100];

	res = ft_strjoin_free("\001", ft_get_color(10, 255, 80), 2);
	res = ft_strjoin_free(res, "\002", 1);
	res = ft_strjoin_free(res, getenv("USER"), 1);
	res = ft_strjoin_free(res, "@", 1);
	res = ft_strjoin_free(res, "minishell\001\033[0m\002:\001", 1);
	res = ft_strjoin_free(res, ft_get_color(80, 80, 255), 3);
	res = ft_strjoin_free(res, "\002", 1);
	res = ft_strjoin_free(res, getcwd(cwd_buffer, 99), 1);
	res = ft_strjoin_free(res, "\001\033[0m\002$ ", 1);
	return (res);
}

t_env	*ft_get_env(char **envp)
{
	t_env	*env;
	char	*name;
	char	*value;
	int		i;
	int		j;

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

int	main(int argc, char **argv, char **envp)
{
	char	*command;
	t_cmd	*parsed_cmd;
	t_env	*env;
	char	*prompt;

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
