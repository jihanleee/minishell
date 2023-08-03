#ifndef MINISHELL_H
# define MINISHELL_H
/*add more headers if needed.*/
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <curses.h>
# include <term.h>
# include <signal.h>
# include <errno.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include "libft.h"

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
	in_file,
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

typedef struct s_pipe
{
	int				in;	//0(stdin), 1(infile), 2(heredoc)
	int				out; //0(stdout), 3(outfile), 4(append)
	char 			*cmd;
	char			**arg;
	char			*infile;
	char			*outfile;
	struct s_pipe	*next;
}				t_pipe;

/*
typedef struct s_list
{
	char	*line;
	s_list	*next;
	//s_list	*prev;
}				t_list;
*/


/*exec*/
int 	non_builtin(t_pipe *cmd_line, char **env, int fds[]);
void	apply_redir(t_pipe *cmd_line);
int		exec_function(t_pipe *cmd_line, char ***env, int end[]);
void 	exec(t_pipe *cmd_line, char **env);
void 	exec_command_line(t_pipe *cmd_line, char **env);



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
t_token	*parse_tokens(t_token **lst, void (*del)(void *));

/*signal.c*/
void	new_prompt(void);
void 	sig_handler(int signum, siginfo_t *info, void *context);
int 	sigaction_set(void);

//---------------------------------------------------------------
/* cd.c */
int		ft_cd(t_pipe **lst, char **env, int fd);

/* echo.c */
int		get_length(char *str);
int		ft_echo(t_pipe **lst, char **env, int fd);

/* env.c */
int		ft_env(t_pipe **lst, char **env, int fd);

/* exit.c */
int		check_int(char *arg);
int		ft_exit(t_pipe **lst, char **env, int fd);

/* export.c */
int	first_is_valid(char c);
void	export_combined(char **combine);
int		get_block_count(t_pipe **lst);
int		fill_blocks(char **combine, t_pipe **lst);
int		ft_export(t_pipe **lst, char **env, int fd);

/* pwd.c */
int		ft_pwd(t_pipe **lst, char **env, int fd);

/* unset.c */
//void	delete_line(char **env, char *string);
int		ft_unset(t_pipe **lst, char **env, int fd);

/* temp_execute.c */
void	test_execute(t_pipe *pipes, char **env, int fd);


/*main.c*/
void	clear_tokens(t_token **lst, void (*del)(void *));
//moved to expander.c for now

#endif
