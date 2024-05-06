/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 17:31:38 by tomoron           #+#    #+#             */
/*   Updated: 2024/05/06 14:27:14 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <readline/readline.h>
# include <readline/history.h>
# include <limits.h>
# include <stdio.h>
# include <sys/wait.h>
# include "../libft/libft.h"
# include "fcntl.h"
# include <sys/stat.h>
# include <signal.h>
# include <stdint.h>
# include <termios.h>
# include <dirent.h>

typedef enum e_cmd_type
{
	CMD,
	PAREN,
	AND,
	OR,
	PIPE,
	RED_O_APP,
	HERE_DOC,
	RED_O,
	RED_I,
	ERR
}	t_cmd_type;

typedef struct s_cmd
{
	t_cmd_type		cmd_type;
	char			*value;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_token
{
	char			*value;
	struct s_token	*next;
}	t_token;

typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_msh
{
	t_env			*env;
	t_token			*tokens;
	t_cmd			*cmds;
	t_cmd			*cmds_head;
	int				**fds;
	int				*pids;
	t_cmd_type		in_type;
	t_cmd_type		out_type;
	int				in_fd;
	int				echoctl;
	int				out_fd;
	int				last_return_code;
	int				locked_return_code;
	char			*here_doc_filename;
}	t_msh;

extern int	g_return_code;

int		get_in_type(t_msh *msh, t_cmd *t_strt, t_cmd *tokens, int here_doc);
int		add_var_to_str(char *res, char **command, t_env *env, int dquote);
t_env	*export_set_env(t_env *env, char *name, char *value, int append);
t_env	*env_add_back(t_env *env, char *name, char *value, int empty);
void	parent(t_msh *msh, int i, int cmd_count, char **cmd_args);
char	*expand_variables(char *command, t_env *env, int *is_var);
t_token	*expand_wildcards(t_token *res, char *value, int is_var);
int		check_tokens_syntax(t_cmd *cmd, t_cmd *last, t_env *env);
t_token	*token_add_back(t_token *res, char *token, int is_var);
int		recursive_filename_check(char *wildcard, char **value);
int		get_variable_expantion_len(char *command, t_env *env);
t_cmd	*cmd_add_back(t_cmd *res, char *cmd, t_cmd_type type);
int		get_var_len(char **command, t_env *env, int dquote);
void	find_cmd_path(t_msh *msh, char **paths, int *found);
void	redirect_output(t_msh *msh, int i, char **cmd_args);
void	redirect_input(t_msh *msh, int i, char **cmd_args);
void	print_syntax_error_bonus(t_cmd *cmd, t_cmd *cmds);
int		filename_corresponds(char *wildcard, char *value);
void	close_all_pipes(t_msh *msh, int cmd_count, int i);
t_token	*parse_cmds_to_token(t_cmd *command, t_env *env);
int		ft_export(t_msh *msh, t_token *cmd, t_env *env);
void	print_env_declare(t_msh *msh, t_env *env_orig);
void	exec_command_bonus(t_msh *msh, char *cmd_str);
void	print_cmd_type(t_cmd_type type, char *value);
t_token	*add_token_back(t_token *res, t_token *next);
t_cmd	*check_cmds_syntax(t_cmd *cmds, t_env *env);
int		cmd_is_builtin(t_msh *msh, char *cmd_token);
void	*here_doc_variables(int write, void *data);
void	get_redirections(t_msh *msh, t_cmd *cmds);
void	ambiguous_redirect(char *str, t_msh *msh);
void	child(t_msh *msh, char **cmd_args, int i);
int		cmd_is_forkable_builtin(char *cmd_token);
t_token	*parse_tokens(char *command, t_env *env);
void	delete_from_env(t_msh *msh, char *name);
char	*ft_get_env(t_env *env, char *var_name);
int		get_out_type(t_msh *msh, t_cmd *cmds);
void	handle_here_doc(t_msh *msh, char *eof);
void	signal_handler_interactive(int signum);
void	signal_handler_here_doc(int signum);
t_token	*parsing_syntax_error(t_token *res);
int		file_access(t_msh *msh, int *found);
void	remove_command_from_msh(t_msh *msh);
void	ft_exit(t_msh *msh, int error_code);
void	sort_wildcards_token(t_token *list);
void	signal_handler_command(int signum);
void	ft_exit(t_msh *msh, int exit_code);
int		get_parenthesis_cmd_len(char *cmd);
char	**split_paths_from_env(t_env *env);
int		add_return_code_to_str(char *res);
void	close_pipe_fds(t_msh *msh, int i);
void	parse_var(t_msh *msh, char *line);
int		get_var_name_len(char *command);
void	handle_minishellrc(t_msh *msh);
char	*get_tmp_file_name(t_msh *msh);
int		cd(t_token *args, t_msh *msh);
int		get_normal_cmd_len(char *cmd);
t_cmd	*get_next_command(t_cmd *cmd);
int		get_args_count(t_token *cmds);
char	**env_to_char_tab(t_env *env);
int		first_is_in_type(t_cmd *cmd);
void	print_msh_struct(t_msh *msh);
int		get_next_arg_len(char *cmd);
int		check_str_syntax(char *cmd);
int		is_operand_type(t_cmd *cmd);
int		contains_newline(char *str);
int		get_cmd_count(t_cmd *cmds);
int		check_var_name(char *name);
char	**get_cmd_args(t_msh *msh);
int		get_cmd_count(t_cmd *cmds);
int		is_parenthesis(t_cmd *cmd);
int		is_output_type(t_cmd *cmd);
int		is_parenthesis(t_cmd *cmd);
void	print_signaled(int status);
int		is_input_type(t_cmd *cmd);
char	*remove_path(char *token);
t_cmd	*parsing_bonus(char *cmd);
void	free_env_cpy(t_env *env);
char	*get_var_name(char *str);
int		exec_builtin(t_msh *msh);
void	get_cmd_path(t_msh *msh);
int		is_cmd_type(t_cmd *cmd);
int		set_echoctl(int value);
int		print_env(t_env *env);
t_cmd	*free_cmd(t_cmd *cmd);
void	free_env(t_env *env);
int		ft_unset(t_msh *msh);
void	free_fds(t_msh *msh);
t_env	*dup_env(t_env *env);
void	sort_env(t_env *env);
void	free_msh(t_msh *msh);
void	free_msh(t_msh *msh);
int		echo(t_token *args);
int		exit_bt(t_msh *msh);
int		is_cmd_char(char c);
int		is_fd_open(int fd);
int		pwd(void);

#endif
