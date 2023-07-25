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
	pipe_op
}			t_token_type;

typedef struct s_word
{
	t_quote_type	type;
	char			*str;
	bool			exp;
	struct s_word	*next;
}				t_word;

typedef struct s_token
{
	t_token_type	type;
	char 			*str;
	struct s_token	*next;
}			t_token;

/*tokenizer.c*/
int		*create_quote_info(char *line);
t_token	*create_tokens(char *line);
int		*create_quote_info(char *line);
char	*find_param(char *p_name, char **envp);

#endif
