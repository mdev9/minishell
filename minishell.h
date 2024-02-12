/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomoron <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 17:31:38 by tomoron           #+#    #+#             */
/*   Updated: 2024/02/11 22:54:49 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <readline/readline.h>
# include <readline/history.h>
# include <limits.h>
# include <stdio.h>//debug	
# include "libft/libft.h"

typedef enum e_token_type
{
	ARG,
	PIPE,
	OR,
	AND
}	t_token_type;

typedef struct s_cmd
{
	t_token_type	type;
	char			*token;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
}	t_env;

extern int	g_return_code;

t_cmd	*ft_cmd_add_back(t_cmd *res, char *token, t_token_type type);
void	ft_free_cmd(t_cmd *cmd);
void	ft_exec_command(t_cmd *cmd, t_env *env);
int		ft_echo(t_cmd *args);
void	ft_exit(t_cmd *args, t_env *env);
t_env	*ft_env_add_back(t_env *env, char *name, char *value);
void	ft_free_env(t_env *env);
int		ft_print_env(t_env *env);
t_cmd	*ft_parse_command(char *command, t_env *env);
int		ft_get_token_len(char *cmd, t_env *env);
int		ft_add_var_to_str(char *res, char **command, t_env *env);
int		get_var_name_len(char *command);
char	*ft_getenv(t_env *env, char *var_name);
int		ft_pwd(void);
#endif
