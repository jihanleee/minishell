#include "minishell.h"

/*IFS characters are '\n', '\t' and ' ' by default in POSIX shell.*/
bool	is_ifs(char c)
{
	if (c == '\n' || c == '\t' || c == ' ')
		return (1);
	else
		return (0);
}

int	len_lvname(char *str)
{
	int	i;

	if (ft_isdigit(str[1]) || str[1] == '?')
		return (2);
	i = 1;
	while (ft_isalnum(str[i]) || str[i] == '_')
		i++;
	return (i);
}

/*
void	split_expansions(t_lexeme *lexemes)
{
	t_lexeme	*current;
	t_lexeme	*next;
	int		i;

	current = lexemes;
	while (current)
	{
		next = current->next;
		//split them!
		i = 0;
		while (current->type != 1 && current->str[i])
		{
			if (current->str[i] == '$' && current->type != 1)
			{
				i++;
				current->exp = TRUE;
			}
			while (current->str[i] && current->str[i] != '$')
				i++;
			if (current->str[i] == '$')
			{
				current->next = new_lexeme(ft_strdup(&(current->str[i])), current->type);
				//protect the malloc
				current->next->next = next;
				current->next->exp = TRUE;
				current->str[i] = '\0';
			}
		}
		current = current->next;
	}
}
*/

/*expansion module tests*/
//OLD - DO NOT USE
/*int	main(int argc, char **argv, char **envp)
{
	t_lexeme	*lexemes;
	t_token	*newtok;
	char	*line;
	int		*info;
	int		*lexeme;
	char	*tok;
	int		i;

	while (1)
	{
		line = readline("%");
		ft_printf("line : %s\n", line);
		lexemes = word_to_lexemes(line);
		read_lexemes(lexemes);
		replace_params(lexemes, envp);
		ft_printf("after expansion\n");
		read_lexemes(lexemes);
		lexeme = lexemes_to_int(lexemes);
		i = 0;
		while (lexeme[i])
			printf("%d\n", lexeme[i++]);
		ft_printf("%d\n", lexemelen(lexemes));
		newtok = lexeme_to_tokens(lexeme);
		read_tokens(newtok);
	}
	return (0);
}
*/
