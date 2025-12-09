/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msakata <msakata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 00:00:00 by student           #+#    #+#             */
/*   Updated: 2025/12/09 20:25:49 by msakata          ###   ########TOKYO.jp  */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <signal.h>
# include <dirent.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <termios.h>
# include "libft.h"

# define PROMPT "minishell$ "

extern int	g_signal_received;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_REDIR_HEREDOC,
	TOKEN_ENV_VAR,
	TOKEN_EXIT_STATUS,
	TOKEN_EMPTY_EXPANSION
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	int				quote_type;
	struct s_token	*next;
}	t_token;

typedef struct s_redir
{
	int				type;
	char			*file;
	struct s_redir	*next;
}	t_redir;

typedef struct s_cmd
{
	char			**args;
	int				*quote_types;
	t_redir			*redirs;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_data
{
	t_env	*env;
	t_cmd	*cmds;
	int		exit_status;
	int		stdin_backup;
	int		stdout_backup;
}	t_data;

/* Lexer */
t_token		*lexer(char *input, t_data *data);
void		free_tokens(t_token *tokens);
t_token		*new_token(t_token_type type, char *value);
void		add_token(t_token **tokens, t_token *new);
char		*handle_quotes(char *input, int *i, char quote_char);
char		*handle_word(char *input, int *i);
void		handle_redirect(char *input, int *i, t_token **tokens);
char		*build_combined_word(char *input, int *i,
				int *quote_type, t_data *data);
int			is_separator(char c);
/* Parser */
t_cmd		*parser(t_token *tokens, t_data *data);
char		**parse_args(t_token **tokens, int **quote_types);
void		free_cmds(t_cmd *cmds);
t_cmd		*new_cmd(void);
void		add_cmd(t_cmd **cmds, t_cmd *new);
t_redir		*new_redir(int type, char *file);
void		add_redir(t_redir **redirs, t_redir *new);

/* Executor */
void		executor(t_cmd *cmds, t_data *data);
void		execute_cmd(t_cmd *cmd, t_data *data);
void		execute_pipeline(t_cmd *cmds, t_data *data);
char		*find_executable(char *cmd, t_env *env);
int			count_cmds(t_cmd *cmds);
void		handle_parent_pipeline(int *prev_pipe, int *pipe_fd, t_cmd *cmd);
void		wait_for_children(int cmd_count, pid_t last_pid, t_data *data);

/* Builtins */
int			is_builtin(char *cmd);
int			execute_builtin(t_cmd *cmd, t_data *data);
int			builtin_echo(char **args);
int			builtin_cd(char **args, t_data *data);
int			builtin_pwd(void);
int			builtin_export(char **args, t_data *data);
int			builtin_unset(char **args, t_data *data);
int			builtin_env(t_env *env);
int			builtin_exit(char **args, t_data *data);

/* Environment */
t_env		*init_env(char **envp);
char		*get_env_value(t_env *env, char *key);
void		set_env_value(t_env **env, char *key, char *value);
void		unset_env_value(t_env **env, char *key);
char		**env_to_array(t_env *env);
void		free_env(t_env *env);

/* Expansion */
char		*expand_variables(char *str, t_data *data);
char		*expand_exit_status(char *str, int exit_status);

/* Redirections */
int			setup_redirections(t_redir *redirs, t_data *data);
int			create_heredoc(char *delimiter, char **filename_out);

/* Signals */
void		setup_signals(void);
void		signal_handler(int signum);
void		signal_handler_heredoc(int signum);

/* Error handling */
void		print_error(char *cmd, char *msg);
void		exit_error(char *msg, int exit_code);

/* Memory management */
void		cleanup_data(t_data *data);
void		free_redirs(t_redir *redirs);

#endif
