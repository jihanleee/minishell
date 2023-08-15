#ifndef MINISHELL_H
# define MINISHELL_H
/*add more headers if needed.*/
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
# include <signal.h>
# include <errno.h>
# include "libft.h"

# define GREEN "\033[32m"

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

/*in : 0(stdin), 1(infile, heredoc)
out : 0(stdout), 3(outfile), 4(append) */
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

/*tokenizer*/
char		*find_param(char *p_name);
void		mark_single_quote(char *line, int *result, int *i);
void		mark_double_quote(char *line, int *result, int *i);
int			*create_quote_info(char *line);
bool		is_op(char c);
bool		is_blank(char c);
int			token_op_len(char *line, int start, int *quote_info);
int			token_word_len(char *line, int start, \
							t_ttype *type, int *quote_info);
void		read_tokens(t_token *current);
t_token		*new_token(char *line, int start, int len, t_ttype type);
int			append_token(t_token **tokens, t_token *new);
void		set_tokentype(t_token *tokens);
int			line_to_token(t_token **tokens, int *quote_info, char *line);
t_token		*create_tokens(char *line);

/*parser.c*/
void		temp_read_tokens(t_token **lst);
int			check_tokens(t_token *tokens);
void		exit_error(char *message, t_token **temp);
void		print_parse_error(t_token **input);
t_token		*parse_tokens(t_token **lst, void (*del)(void *));

/*expander*/
void		clear_tokens(t_token **lst, void (*del)(void *));
char		*assign_single_quote(char *line, int *i, int *quote_info, \
								t_qtype *type);
char		*assign_double_quote(char *line, int *i, int *quote_info, \
								t_qtype *type);
char		*assign_non_quote(char *line, int *i, int *quote_info, \
								t_qtype *type);
void		clear_lexemes(t_lexeme **lst, void (*del)(void *));
t_lexeme	*new_lexeme(char *str, t_qtype type);
int			append_lexeme(t_lexeme **lexemes, t_lexeme *new);
void		read_lexemes(t_lexeme *current);
int			len_lvname(char *str);
void		split_expansions(t_lexeme *lexemes);
t_lexeme	*word_to_lexemes(char *str);
void		replace_params(t_lexeme *lexemes);
int			lexemelen(t_lexeme *lexemes, t_ttype type);
bool		is_ifs(char c);
t_token		*new_expanded_token(int *lexeme, int start, int len);
int			*lexemes_to_int(t_lexeme *lexemes, t_ttype type);
t_token		*iword_to_tokens(int *lexeme);
t_token		*token_to_etoken(t_token *old);
void		expansion(t_token **tokens);
void		read_jobs(t_job *jobs);
char		**extract_arg(t_token **tokens);
void		create_heredoc(const char *delim, bool hereq);
void		open_file_redir(t_token *token);
void		point_prev_job(t_job *jobs);
t_job		*extract_jobs(t_token *tokens);
void		clear_jobs(t_job *current);

/*signal.c*/
void		new_prompt(void);
void		sig_handler(int signum, siginfo_t *info, void *context);
int			sigaction_set(void);

/* cd.c */
int			ft_cd(t_job **lst, int fd);

/* echo.c */
int			get_length(char *str);
void		ft_echo(t_job *current, int fd);

/* env.c */
void		ft_env(t_job **lst, int fd);

/* exit.c */
int			check_int(char *arg);
int			get_exit_num(t_job current);
int			ft_exit(t_job **lst, int fd);

/* export.c */
int			first_is_valid(char c);
int			middle_is_valid(char c);
void		export_combined(char **combine);
int			get_block_count(t_job **lst);
int			fill_blocks(char **combine, t_job **lst);
int			ft_export(t_job **lst, int fd);

/* pwd.c */
int			ft_pwd(t_job **lst, int fd);

/* unset.c */
//void	delete_line(char *string);
//void	ft_unset(t_job **lst, int fd);
//int			ft_unset(t_job **lst, int fd);
int		unset_strncmp(const char *s1, const char *s2, size_t n);
void	ft_unset(t_job **lst, int fd);

/*execution*/
void		free_arrays(char **str);
void		error_exit(char *str, int exit_status, t_job *job);
char		**bin_path(t_job *job);
char		*file_path(char *cmd);
char		*find_cmd_path(char *cmd, t_job *job);
char		**get_argv(t_job *jobs);
int			redirect_fds(t_job *job);
void		non_builtin_child(t_job *job);
int			exec_builtin(t_job *cmd_line, int fd);
void		builtin(t_job *job);
int			check_builtin(char *cmd);
void		execute_jobs(t_job *jobs);
char		**get_envp();

/*env_var*/
void	init_env_var(char **envp);
void	read_env();
t_env	**get_env_address();
void	clear_env();

#endif
