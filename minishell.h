/*double inclusion protection*/
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

typedef enum e_quote_type
{
	non_quoted,
	single_quoted,
	double_quoted
}			t_quote_type;

/*a word consists of consecutive characters,
splitted by non-quoted quotes, following
the quoting rules of bash reference manual.
i.e) if a string given as a token is
	"foo"bar"I'm baz"''
	then the struct array is
	words[0].word == foo // double quoted 
	words[1].word == bar // non-quoted
	words[2].word == I'm baz // double quoted 
	words[3].word == (empty string) // single quoted
*/

typedef struct s_word
{
	t_quote_type	quote_type;
	char			*word;
}				t_word;

/*a token is separated by non-quoted space characters,
consists of the words described above.*/
typedef struct s_token
{
	//token type?
	t_word	*words;
}			t_token;

#endif
