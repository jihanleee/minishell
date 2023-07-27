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
		if (current->exp == 1)
			ft_printf("exp:\tTRUE\n");
		else
			ft_printf("exp:\tFALSE\n");
		current = current->next;
		i++;
	}
}

void	split_expansions(t_word *words)
{
	t_word	*current;
	t_word	*next;
	int		i;

	current = words;
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
				current->next = new_word(ft_strdup(&(current->str[i])), current->type);
				//protect the malloc
				current->next->next = next;
				current->next->exp = TRUE;
				current->str[i] = '\0';
			}
		}
		current = current->next;
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
	split_expansions(result);
	return (free(quote_info), result);
}



void	replace_params(t_word *words, char **envp)
{
	t_word	*current;
	char	*newstr;

	current = words;
	while (current)
	{
		ft_printf("%s\n", current->str);
		newstr = 0;
		if (current->exp == TRUE)
		{
			newstr = find_param(&(current->str[1]), envp);
			if (newstr)
			{
				current->p_found = TRUE;
				free(current->str);
				current->str = newstr;
			}
		}
		current = current->next;
	}
}

int	wordlen(t_word *words)
{
	t_word	*current;
	int		*word;
	int		i;
	int		count;

	count = 0;
	current = words;
	while (current)
	{
		i = 0;
		while (current->str[i])
		{
			if (current->exp == TRUE && current->p_found == FALSE)
				break;
			i++;
			count++;
		}
		current = current->next;
	}
	return (count);
}

/*IFS characters are '\n', '\t' and ' ' by default in POSIX shell.*/
bool	is_ifs(char c)
{
	if (c == '\n' || c == '\t' || c == ' ')
		return (1);
	else
		return (0);
}

t_token	*new_expanded_token(int *word, int start, int len)
{
	t_token *new;
	int		i;

	new = (t_token *)ft_calloc(1, sizeof (t_token));
	if (new == NULL)
		return (NULL);
	if (len)
		new->str = ft_calloc(sizeof(char), len + 1);
	else
		return (NULL);
	//ft_printf("len %d\n", len);
	if (new->str == NULL)
		return (free(new), NULL);
	i = 0;
	while (i < len)
		new->str[i++] = (char)word[start++];
	return (new);
}
/*returns an int array in which IFS will be translated into -1,
and used as separator to perform field splitting.
if not IFS, each element is stored as ascii value of the original character.
the returned array is NULL-terminated.*/
int	*words_to_int(t_word *words)
{
	t_word	*current;
	int		*word;
	int		i;
	int		j;

	j = 0;
	word = (int *)ft_calloc(wordlen(words) + 1, sizeof(int));
	if (word == 0)
		return (NULL);
	current = words;
	while (current)
	{
		while (current->exp == TRUE && current->p_found == FALSE)
			current = current->next;
		if (current == 0)
			break ;
		i = 0;
		while (current->str[i])
		{
			ft_printf("beh");
			if (is_ifs(current->str[i]) && current->type == 0)
				word[j++] = -1;
			else
				word[j++] = current->str[i];
			i++;
		}
		current = current->next;
	}
	return (word);
}
/*lets use this structure*/
/*
int	line_to_token(t_token **tokens, int *quote_info, char *line)
{
	int				len;
	int				start;
	t_token_type	type;

	start = 0;
 	*tokens = 0;
	while (line[start])
	{
		if (is_blank(line[start]) && !quote_info[start])
			start++;
		else
		{
			if (is_op(line[start]) && !quote_info[start])
				len = token_op_len(line, start, quote_info);
			else
				len = token_word_len(line, start, &type, quote_info);
			if (append_token(tokens, new_token(line, start, len, type)) == -1)
				return (clear_tokens(tokens, free), (free(quote_info), -1));
			start += len;
		}
	}
	return (0);
}
*/

t_token	*word_to_tokens(int *word)
{
	t_token	*result;
	int		start;
	int		len;

	result = 0;
	start = 0;
	while (word[start])
	{
		while(word[start] == -1)
			start++;
		len = 0;
		while (word[start + len] && word[start + len] != -1)
			len++;
		if (append_token(&result, new_expanded_token(word, start, len)) == -1)
			return (clear_tokens(result, free), (free(word), -1));
		start += len;
	}
	return (result);
}

/*expansion module tests*/
/* int	main(int argc, char **argv, char **envp)
{
	t_word	*words;
	t_token	*newtok;
	char	*line;
	int		*info;
	int		*word;
	char	*tok;
	int		i;

	while (1)
	{
		line = readline("%");
		ft_printf("line : %s\n", line);
		words = split_words(line);
		read_words(words);
		replace_params(words, envp);
		ft_printf("after expansion\n");
		read_words(words);
		word = words_to_int(words);
		i = 0;
		while (word[i])
			printf("%d\n", word[i++]);
		ft_printf("%d\n", wordlen(words));
		newtok = word_to_tokens(word);
		read_tokens(newtok);
	}
	return (0);
} */