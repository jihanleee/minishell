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

typedef enum e_quote_type
{
	uq,
	sq,
	dq
}			t_quote_type;

typedef enum e_token_type
{
	word,
	in,
	heredoc,
	out,
	append,
	pipe_op,
	amb_redir
}			t_token_type;

typedef struct s_lexeme
{
	t_quote_type	type;
	char			*str;
	bool			exp;
	bool			p_found;
	struct s_lexeme	*next;
}				t_lexeme;

typedef struct s_token
{
	t_token_type	type;
	char 			*str;
	struct s_token	*next;
}			t_token;

typedef struct s_job
{
	int				in;	//0(stdin), 1(infile), 2(heredoc)
	int				out; //0(stdout), 3(outfile), 4(append)
	char 			*cmd;
	char			**arg;
	char			*infile;
	char			*outfile;
	struct s_job	*next;
	struct s_job	*prev;
	int				pipefd[2];
}				t_job;

/*
typedef struct s_list
{
	char	*line;
	s_list	*next;
	//s_list	*prev;
}				t_list;
*/

extern int g_exit_stat;
/*find_param.c*/
char	*find_param(char *p_name, char **envp);

/*tokenizer.c*/
void	read_tokens(t_token *current);
int		*create_quote_info(char *line);
t_token	*create_tokens(char *line);
int		*create_quote_info(char *line);
char	*find_param(char *p_name, char **envp);
int		append_token(t_token **tokens, t_token *new);

/*parser.c*/
void	temp_read_tokens(t_token **lst);
int		check_tokens(t_token *tokens);
void	exit_error(char *message, t_token **temp);
void	print_parse_error(t_token **input);
t_token	*parse_tokens(t_token **lst, void (*del)(void *));

/*expander.c*/
void	clear_tokens(t_token **lst, void (*del)(void *));
char	*assign_single_quote(char *line, int *i, int *quote_info, t_quote_type *type);
char	*assign_double_quote(char *line, int *i, int *quote_info, t_quote_type *type);
char	*assign_non_quote(char *line, int *i, int *quote_info, t_quote_type *type);
void	clear_lexemes(t_lexeme **lst, void (*del)(void *));
t_lexeme	*new_lexeme(char *str, t_quote_type type);
int	append_lexeme(t_lexeme **lexemes, t_lexeme *new);
void	read_lexemes(t_lexeme *current);
int    len_lvname(char *str);
void    split_expansions(t_lexeme *lexemes);
t_lexeme	*word_to_lexemes(char *str);
void	replace_params(t_lexeme *lexemes, char **envp);
int	lexemelen(t_lexeme *lexemes, t_token_type type);
bool	is_ifs(char c);
t_token	*new_expanded_token(int *lexeme, int start, int len);
int	*lexemes_to_int(t_lexeme *lexemes, t_token_type type);
t_token	*iword_to_tokens(int *lexeme);
t_token	*token_to_etoken(t_token *old, char **envp);
void	expansion(t_token **tokens, char **envp);
void	read_jobs(t_job *jobs);
char	**extract_arg(t_token **tokens);
void	create_heredoc(const char *delim);
void	open_file_redir(t_token *token);
t_job	*extract_jobs(t_token *tokens);
void	clear_jobs(t_job **lst);

/*signal.c*/
void	new_prompt(void);
void 	sig_handler(int signum, siginfo_t *info, void *context);
int 	sigaction_set(void);

//---------------------------------------------------------------
/* cd.c */
int	ft_cd(t_job **lst, char **env, int fd);

/* echo.c */
int		get_length(char *str);
void	ft_echo(t_job **lst, char **env, int fd);

/* env.c */
void	ft_env(t_job **lst, char **env, int fd);

/* exit.c */
int		check_int(char *arg);
int		get_exit_num(t_job current);
int		ft_exit(t_job **lst, char **env, int fd);

/* export.c */
int		first_is_valid(char c);
int		middle_is_valid(char c);
void	export_combined(char **combine);
int		get_block_count(t_job **lst);
int		fill_blocks(char **combine, t_job **lst);
int		ft_export(t_job **lst, char **env, int fd);

/* pwd.c */
int		ft_pwd(t_job **lst, char **env, int fd);

/* unset.c */
//void	delete_line(char **env, char *string);
//void	ft_unset(t_job **lst, char **env, int fd);
int	ft_unset(t_job **lst, char **env, int fd);

/*main.c*/
//moved to expander.c for now
/*execution.c*/
void	execute_jobs(t_job *jobs, char **envp);

#endif
