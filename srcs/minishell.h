/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 17:31:38 by tomoron           #+#    #+#             */
/*   Updated: 2024/03/22 16:44:54 by tomoron          ###   ########.fr       */
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
# include <sys/stat.h>
# include <signal.h>

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
	struct s_alias		*aliases;
	struct s_env		*env;
	struct s_cmd		*cmds;
	int					**fds;
	int					*pids;
	enum e_token_type	in_type;
	enum e_token_type	out_type;
	int					in_fd;
	int					out_fd;
}	t_msh;

extern int	g_return_code;

t_cmd	*cmd_add_back(t_cmd *res, char *token, t_token_type type);
void	free_cmd(t_cmd *cmd);
void	exec_commands(t_msh *msh);
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
char	**split_paths_from_env(t_env *env);
void	find_cmd_path(t_msh *msh, char **paths, int *found);
void	get_cmd_path(t_msh *msh);
void	handle_here_doc(t_msh *msh, char *eof);
int		ft_unset(t_msh *msh);
void	get_in_type(t_msh *msh, t_cmd *cmds);
void	get_out_type(t_msh *msh, t_cmd *cmds);
int		first_is_in_type(t_msh *msh);
void	redirect_input(t_msh *msh, int i);
void	redirect_output(t_msh *msh, int i);
void	child(t_msh *msh, char **cmd_args, int i);
void	parent(t_msh *msh, int i, int cmd_count);
void	free_msh(t_msh *msh);
void	ft_exit(t_msh *msh, int exit_code);
int		cmd_is_builtin(t_msh *msh, char *cmd_token);
int		exec_builtin(t_msh *msh);
int		get_cmd_count(t_cmd *cmds);
int		get_args_count(t_cmd *cmds);
char	**get_cmd_args(t_msh *msh);
void	remove_command_from_msh(t_msh *msh);
int		file_access(t_msh *msh, int *found);
char	*remove_path(char *token);
void	signal_handler_interactive(int signum);
void	signal_handler_command(int signum);

#endif
