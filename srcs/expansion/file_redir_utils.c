#include "minishell.h"

char	*gnl_heredoc(bool hereq, char **line)
{
	t_lexeme	*lexemes;
	t_lexeme	*current;
	int			i;
	int			j;

	if (hereq || !*line)
		return (*line);
	lexemes = new_lexeme(ft_strdup(*line), 0);
	split_expansions((free(*line), lexemes));
	replace_params(lexemes);
	*line = (char *)ft_calloc(lexemelen(lexemes, 0) + 1, sizeof (char));
	i = 0;
	current = lexemes;
	while (*line && current)
	{
		j = 0;
		while (current->str[j] && !(!current->p_found && current->exp))
			(*line)[i++] = current->str[j++];
		current = current->next;
	}
	clear_lexemes(&lexemes, free);
	return (*line);
}

char	*readline_nl(char *str)
{
	char	*line;
	char	*withnl;

	line = readline(str);
	withnl = ft_strjoin(line, "\n");
	free(line);
	return (withnl);
}
