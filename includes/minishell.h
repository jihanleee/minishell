/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jihalee <jihalee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 12:11:59 by solee2            #+#    #+#             */
/*   Updated: 2023/09/26 21:48:21 by jihalee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <curses.h>
# include <term.h>
# ifndef __USE_POSIX
#  define __USE_POSIX
#  include <signal.h>
# endif
# include <errno.h>
# include <dirent.h>
# include "libft.h"
# define GREEN "\e[1;32m"
# define BACK "\e[0m"

typedef struct s_env
{
	char			*str;
	struct s_env	*next;
}				t_env;

typedef enum e_quote_type
{
	uq,
	sq,
	dq
}			t_qtype;

typedef enum e_token_type
{
	word,
	in,
	heredoc,
	out,
	append,
	pipe_op,
	amb_redir
}			t_ttype;

typedef struct s_lexeme
{
	t_qtype			type;
	char			*str;
	bool			exp;
	bool			p_found;
	struct s_lexeme	*next;
}				t_lexeme;

typedef struct s_token
{
	t_ttype			type;
	bool			hereq;
	char			*str;
	struct s_token	*next;
}			t_token;

typedef struct s_job
{
	int				in;
	int				out;
	char			*cmd;
	char			**arg;
	char			*infile;
	char			*outfile;
	struct s_job	*next;
	struct s_job	*prev;
	int				pipefd[2];
}				t_job;

extern int	g_exit_stat;

/* --------------------tokenizer--------------------*/
/* create_tokens.c */
t_token		*new_token(char *line, int start, int len, t_ttype type);
int			append_token(t_token **tokens, t_token *new);
void		set_tokentype(t_token *tokens);
int			line_to_token(t_token **tokens, int *quote_info, char *line);
t_token		*create_tokens(char *line);

/* token_substr_len.c */
bool		is_op(char c);
bool		is_blank(char c);
int			token_op_len(char *line, int start, int *quote_info);
int			token_word_len(char *line, int start, \
							t_ttype *type, int *quote_info);

/* token_utils.c */
void		mark_single_quote(char *line, int *result, int *i);
void		mark_double_quote(char *line, int *result, int *i);
int			*create_quote_info(char *line);

/* --------------------parsing--------------------*/
/* parser.c */
void		change_type(t_token *current, t_token *next);
t_token		*parse_tokens(t_token **lst, void (*del)(void *));
int			token_error_check(t_token *tokens);
int			check_tokens(t_token *tokens);
void		print_parse_error(t_token **input);

/* --------------------expansion--------------------*/
/* expander.c */
void		split_expansions(t_lexeme *lexemes);
void		expansion(t_token **tokens);

/* token_conversion1.c */
t_token		*new_expanded_token(int *lexeme, int start, int len);
int			*lexemes_to_int(t_lexeme *lexemes, t_ttype type);
t_token		*iword_to_tokens(int *lexeme);

/* token_conversion2.c */
t_token		*token_to_etoken(t_token *old);

/* assign_quotes.c */
char		*assign_single_quote(char *line, int *i, int *quote_info, \
								t_qtype *type);
char		*assign_double_quote(char *line, int *i, int *quote_info, \
								t_qtype *type);
char		*assign_non_quote(char *line, int *i, int *quote_info, \
								t_qtype *type);

/* replace.c */
void		replace_params(t_lexeme *lexemes);

/* lexemes.c */
t_lexeme	*new_lexeme(char *str, t_qtype type);
int			append_lexeme(t_lexeme **lexemes, t_lexeme *new);
int			lexemelen(t_lexeme *lexemes, t_ttype type);
t_lexeme	*word_to_lexemes(char *str);

/* jobs.c */
void		point_prev_job(t_job *jobs);
t_job		*extract_jobs(t_token *tokens);
char		**extract_arg(t_token **tokens);
void		assign_cmd_arg(t_token *current, t_job *cur_result);

/* find_paramc. */
char		*find_param(char *p_name);

/* file_redir.c */
void		create_heredoc(const char *delim, bool hereq, char *randname);
int			open_file_redir(t_token **tokens);
char		*gnl_heredoc(bool hereq, char **line);
char		*readline_nl(char *str);

/* expansion_utils.c */
bool		is_ifs(char c);
int			len_lvname(char *str);
void		set_random_filename(char *str);
/* clear.c */
void		clear_tokens(t_token **lst, void (*del)(void *));
void		clear_lexemes(t_lexeme **lst, void (*del)(void *));
void		clear_jobs(t_job *current);

/* --------------------execution--------------------*/
/* execution.c */
void		non_builtin_child(t_job *job);
int			exec_builtin(t_job *cmd_line, int fd, bool child);
void		builtin_child(t_job *job);
void		execute_pipes(t_job *current);
void		execute_jobs(t_job *jobs);

/* exec_utils1.c */
void		error_exit(char *str, int exit_status, t_job *job);
char		**bin_path(t_job *job);
char		*file_path(char *cmd, t_job *job);
char		*find_cmd_path(char *cmd, t_job *job);

/* exec_utils2.c */
int			check_builtin(char *cmd);
char		**get_envp(void);
char		**get_argv(t_job *jobs);
int			redirect_fds(t_job *job);
int			get_child_status(int stat);

/* exec_free.c*/
void		free_arrays(char **str);
void		free_empty_cmd(char *cmd_path, t_job *job);
void		close_pipes(t_job *current);

/* --------------------signal--------------------*/
/*signal.c*/
void		sig_handler_prompt(int signum, siginfo_t *info, void *context);
int			sigaction_set_prompt(void);
int			sigaction_set_parent(void);
void		sig_handler_parent(int signum, siginfo_t *info, void *context);
int			sigaction_set_heredoc_parent(void);
/* signal_heredoc.c */
int			sigaction_set_heredoc(void);
void		sig_handler_heredoc(int signum, siginfo_t *info, void *context);

/* --------------------cd--------------------*/
/* cd.c */
void		change_update(char *new);
void		ft_cd(t_job **lst);

/* cd_paths.c */
void		cd_old_path(void);
void		cd_normal_path(t_job *current);
void		cd_to_home(void);
void		cd_absolute_path(t_job *current);
void		cd_absolute_path_two(t_job *current);

/* cd_utils.c */
int			cd_strncmp(const char *s1, const char *s2, size_t n);
void		cd_unset(char *arg);
bool		is_dir(char *path);
void		cd_to_root(void);

/* cd_permission_error.c */
void		cd_nonexist_error(char *str);
void		cd_dir_error(char *str);
void		cd_permission_error(char *str);
int			invalid_cd(char *path);

/* --------------------echo--------------------*/
int			print_args(t_job *current, int i, int fd);
void		ft_echo(t_job *current, int fd);

/* --------------------env--------------------*/
void		ft_env(t_job **lst, int fd);

/* --------------------exit--------------------*/
int			check_int(char *arg);
int			cnt_arg(t_job *current);
void		free_exit(int exit_status, t_job *job, int fd);
void		exit_error_cases(int arg_cnt, t_job *current, int fd);
int			ft_exit(t_job **lst, int fd);
bool		is_overflow(char *arg);
/* --------------------export--------------------*/
/* export.c */
int			validate_and_add(t_job **lst);
char		*get_key(char *str);
int			add_to_env(char *str);
int			ft_export(t_job **lst, int fd);

/* export_utils.c */
int			first_is_valid(char c);
int			middle_is_valid(char *str);
int			middle_error_case(char *str);
void		export_error(char *str);
void		export_env(int fd);

/*export_unset*/
void		pop_env_node(t_env **env, t_env **current, t_env **prev);
int			export_unset(char *key);
/* --------------------pwd--------------------*/
/* pwd.c */
int			ft_pwd(int fd);

/* --------------------unset--------------------*/
/* unset.c */
int			unset_strncmp(const char *s1, const char *s2, size_t n);
t_env		*check_first(t_env *env, char *value);
void		ft_unset(t_job **lst);

/* --------------------others--------------------*/
/* debug.c */
void		read_jobs(t_job *jobs);
void		read_lexemes(t_lexeme *crnt);
void		read_tokens(t_token *current);

/* env_var.c */
t_env		**get_env_address(void);
void		read_env(void);
void		clear_env(void);
void		init_env_var(char **envp);

/* main.c */
t_token		**get_token_address(void);

#endif