/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 21:59:20 by tomoron           #+#    #+#             */
/*   Updated: 2024/05/06 14:25:27 by tomoron          ###   ########.fr       */
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
	if (getenv("USER"))
	{
		res = ft_strjoin_free(res, getenv("USER"), 1);
		res = ft_strjoin_free(res, "@", 1);
	}
	res = ft_strjoin_free(res, "hell\001\033[0m\002:\001", 1);
	res = ft_strjoin_free(res, ft_get_color(80, 80, 255), 3);
	res = ft_strjoin_free(res, "\033[1m\002", 1);
	cwd = getcwd(cwd_buffer, 99);
	if (cwd && ft_get_env(env, "HOME") && !ft_strncmp(cwd_buffer,
			ft_get_env(env, "HOME"), ft_strlen(ft_get_env(env, "HOME"))))
	{
		cwd += ft_strlen(getenv("HOME")) - 1;
		cwd[0] = '~';
	}
	if (cwd)
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
		env = env_add_back(env, name, value, 0);
		if (!name || !value)
			free_env(env);
		if (!name || !value)
			return (0);
		envp++;
	}
	return (env);
}

t_env	*add_shlvl(t_env *env)
{
	int		nb;
	char	*tmp;

	tmp = ft_get_env(env, "SHLVL");
	if (!tmp)
		nb = 0;
	else
		nb = ft_atoi(tmp);
	nb++;
	if (nb < 0)
		nb = 0;
	env = export_set_env(env, ft_strdup("SHLVL"), ft_itoa(nb), 0);
	return (env);
}

int	init_minishell(t_msh **msh, int argc, char **argv, char **envp)
{
	struct termios	t_p;

	*msh = ft_calloc(1, sizeof(t_msh));
	if (!*msh)
		exit(1);
	(void)argc;
	(void)argv;
	if (isatty(1))
	{
		tcgetattr(1, &t_p);
		(*msh)->echoctl = t_p.c_lflag & ECHOCTL;
	}
	(*msh)->env = get_env(envp);
	(*msh)->env = add_shlvl((*msh)->env);
	rl_catch_signals = 0;
	g_return_code = 0;
	signal(SIGINT, signal_handler_interactive);
	signal(SIGQUIT, signal_handler_interactive);
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	char	*commands;
	char	*prompt;
	t_msh	*msh;

	commands = (char *)1;
	init_minishell(&msh, argc, argv, envp);
	while (commands)
	{
		prompt = get_prompt(msh->env);
		if (!prompt)
			ft_exit(msh, 1);
		commands = readline(prompt);
		free(prompt);
		add_history(commands);
		exec_command_bonus(msh, commands);
	}
	printf("exit\n");
	ft_exit(msh, g_return_code);
}
