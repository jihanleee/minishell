#ifndef MINISHELL_H
# define MINISHELL_H
/*add more headers if needed.*/
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <curses.h>
# include <term.h>
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
	hdoc_delim,
	out_file,
	app_file
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

/*find_param.c*/
char	*find_param(char *p_name, char **envp);

/*tokenizer.c*/
void	read_tokens(t_token *current);
int		*create_quote_info(char *line);
t_token	*create_tokens(char *line);
int		*create_quote_info(char *line);
char	*find_param(char *p_name, char **envp);

/*parser.c*/
void	temp_read_tokens(t_token **lst);
int		check_tokens(t_token *tokens);
void	exit_error(char *message, t_token **temp);
t_token	*parse_tokens(t_token **lst, void (*del)(void *));

/*main.c*/
void	clear_tokens(t_token **lst, void (*del)(void *));

#endif
