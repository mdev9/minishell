/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 17:31:38 by tomoron           #+#    #+#             */
/*   Updated: 2024/02/21 19:13:47 by marde-vr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <readline/readline.h>
# include <readline/history.h>
# include <limits.h>
# include <stdio.h>//debug	
# include <sys/wait.h>
# include "../libft/libft.h"
# include "fcntl.h"

typedef enum e_token_type
{
	ARG,
	PIPE,
	RED_O,
	RED_O_APP,
	RED_I,
	HERE_DOC,
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

typedef struct s_alias
{
	char			*name;
	char			*value;
	struct s_alias	*next;
}	t_alias;

typedef struct s_msh
{
	struct s_alias	*aliases;
	struct s_env	*env;
	struct s_cmd	*cmds;
	int				*fds;
}	t_msh;

extern int	g_return_code;

t_cmd	*cmd_add_back(t_cmd *res, char *token, t_token_type type);
void	free_cmd(t_cmd *cmd);
void	exec_command(t_msh *msh);
int		echo(t_cmd *args);
void	exit_bt(t_msh *msh);
t_env	*env_add_back(t_env *env, char *name, char *value);
void	free_env(t_env *env);
int		print_env(t_env *env);
t_cmd	*parse_command(char *command, t_env *env);
int		get_token_len(char *cmd, t_env *env);
int		add_var_to_str(char *res, char **command, t_env *env);
int		get_var_name_len(char *command);
char	*ft_get_env(t_env *env, char *var_name);
int		pwd(void);
int		is_cmd_char(char c);
void	print_parsed_cmd(t_cmd *cmd);//debug
void	ft_exit(t_msh *msh, int error_code);
char	**env_to_char_tab(t_env *env);
void	handle_minishellrc(t_msh *msh);
t_cmd	*handle_alias(t_msh *msh);	
int		cd(t_cmd *args);
int		alias(t_msh *msh);
void	free_alias(t_alias *alias);
char	*get_alias(t_alias *alias, char *var_name);
t_alias	*alias_add_back(t_alias *alias, char *name, char *value);
int		unalias(t_msh *msh);
int		ft_export(t_msh *msh);
void	free_msh(t_msh *msh);

#endif
