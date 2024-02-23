/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 21:59:20 by tomoron           #+#    #+#             */
/*   Updated: 2024/02/23 14:06:24 by marde-vr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		g_return_code = 0;

char	*get_prompt(t_env *env)
{
	char	*res;
	char	cwd_buffer[100];
	char	*cwd;

	res = ft_strjoin_free("\001", ft_get_color(10, 255, 80), 2);
	res = ft_strjoin_free(res, "\033[1m\002", 1);
	res = ft_strjoin_free(res, getenv("USER"), 1);
	res = ft_strjoin_free(res, "@", 1);
	res = ft_strjoin_free(res, "minishell\001\033[0m\002:\001", 1);
	res = ft_strjoin_free(res, ft_get_color(80, 80, 255), 3);
	res = ft_strjoin_free(res, "\033[1m\002", 1);
	cwd = getcwd(cwd_buffer, 99);
	if (ft_get_env(env, "HOME") && !ft_strncmp(cwd_buffer, ft_get_env(env,
				"HOME"), ft_strlen(ft_get_env(env, "HOME"))))
	{
		cwd += ft_strlen(getenv("HOME")) - 1;
		cwd[0] = '~';
	}
	res = ft_strjoin_free(res, cwd, 1);
	res = ft_strjoin_free(res, "\001\033[0m\002$ ", 1);
	return (res);
}

t_env	*get_env(char **envp)
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
		env = env_add_back(env, name, value);
		if (!name || !value)
			free_env(env);
		if (!name || !value)
			return (0);
		envp++;
	}
	return (env);
}

int	init_minishell(t_msh **msh, int argc, char **argv, char **envp)
{
	*msh = ft_calloc(1, sizeof(t_msh));
	if (!*msh)
		ft_exit(*msh, 1);
	(void)argc;
	(void)argv;
	(*msh)->env = get_env(envp);
	(*msh)->aliases = 0;
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	char	*command;
	char	*prompt;
	t_msh	*msh;

	command = (char *)1;
	init_minishell(&msh, argc, argv, envp);
	//handle_minishellrc(msh);
	while (msh->env && command)
	{
		prompt = get_prompt(msh->env);
		if (!prompt)
			exit(STDIN_FILENO);
		command = readline(prompt);
		free(prompt);
		add_history(command);
		msh->cmds = parse_command(command, msh->env);
		free(command);
		//print_parsed_cmd(parsed_cmd);//debug
		msh->cmds = handle_alias(msh);
		//print_parsed_cmd(parsed_cmd);//debug
		exec_command(msh);
		free_cmd(msh->cmds);
	}
	rl_clear_history();
	free_msh(msh);
	return (g_return_code);
}
