#include "minishell.h"

char	*assign_single_quote(char *line, int *i, int *quote_info, t_quote_type *type)
{
	int		start;
	int		size;
	char	*result;

	*type = sq;
	size = 0;
	(*i)++;
	start = *i;
	if (line[(*i)] == '\'' && quote_info[*i] == 1)
	{
		result = ft_substr(line, start, 0);
		return ((*i)++, result);
	}
	while (line[(*i)] && line[(*i)] != '\'' && quote_info[*i] == 1)
	{
		size++;
		(*i)++;
	}
	(*i)++;
	result = ft_substr(line, start, size);
	return (result);
}

char	*assign_double_quote(char *line, int *i, int *quote_info, t_quote_type *type)
{
	int		start;
	int		size;
	char	*result;

	*type = dq;
	size = 0;
	(*i)++;
	start = *i;
	if (line[(*i)] == '\"' && quote_info[*i] == 2)
	{
		result = ft_substr(line, start, 0);
		return ((*i)++, result);
	}
	while (line[(*i)] && line[(*i)] != '\"' && quote_info[*i] == 2)
	{
		size++;
		(*i)++;
	}
	(*i)++;
	result = ft_substr(line, start, size);
	return (result);
}

char	*assign_non_quote(char *line, int *i, int *quote_info, t_quote_type *type)
{
	int		start;
	int		size;
	char	*result;

	*type = uq;
	size = 0;
	start = *i;
	while (line[*i] && quote_info[*i] == 0)
	{
		(*i)++;
		size++;
	}
	result = ft_substr(line, start, size);
	return (result);
}

void	clear_words(t_word **lst, void (*del)(void *))
{
	t_word	*current;
	t_word	*next;

	if (lst == 0)
		return ;
	if (del == 0)
		return ;
	current = *lst;
	while (current)
	{
		next = current->next;
		if (current->str)
			del(current->str);
		free(current);
		current = next;
	}
	*lst = 0;
}

t_word	*new_word(char *str, t_quote_type type)
{
	t_word *new;
	
	new = (t_word *)ft_calloc(1, sizeof (t_word));
	if (new == NULL)
		return (NULL);
	new->str = str;
	if (new->str == NULL)
		return (free(new), NULL);
	new->type = type;
	return (new);
}

int	append_word(t_word **words, t_word *new)
{
	t_word	*current;

	if (new == 0)
		return (-1);
	if (*words == 0)
		*words = new;
 	else
	{
		current = *words;
		while (current->next)
			current = current->next;
		current->next = new;
	}
	return (0);
}

void	read_words(t_word *current)
{
	int		i;
	
	i = 0;
	while (current)
	{
		ft_printf("index:\t%d\n", i);
		ft_printf("str:\t%s\n", current->str);
		ft_printf("type:\t%d\n", current->type);
		current = current->next;
		i++;
	}
}

t_word	*split_words(char *str)
{
	int				i;
	char			*newstr;
	t_word			*result;
	t_quote_type	type;
	int				*quote_info;

	quote_info = create_quote_info(str);
	if (quote_info == 0)
		return (NULL);
	result = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' && quote_info[i] == 1)
			newstr = assign_single_quote(str, &i, quote_info, &type);
		else if (str[i] == '\"' && quote_info[i] == 2)
			newstr = assign_double_quote(str, &i, quote_info, &type);
		else if (str[i] && quote_info[i] == 0)
			newstr = assign_non_quote(str, &i, quote_info, &type);
		append_word(&result, new_word(newstr, type));
	}
	read_words(result);
	return (free(quote_info), 0);
}

/* t_token	*expand(t_token *tokens)
{
	t_token	*new;
	int		i;
	char	str;

	i = 0;
	while (str[i])
	{

	}
} */

/* int	main(int argc, char **argv, char **envp)
{
	t_token	*tokens;
	char	*line;
	int		*info;
	int		i;
	char	*tok;

	while (1)
	{
		line = readline("%");
		printf("line : %s", line);
		split_words(line);
	}
	return (0);
} */