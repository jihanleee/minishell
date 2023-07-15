#include "minishell.h"

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	mark_single_quote(char *line, int *result, int *i)
{
	int	start;

	start = *i;
	result[(*i)++] = 1;
	while (line[(*i)] && line[(*i)] != '\'')
		result[(*i)++] = 1;
	if (line[*i] == 0)
	{
		*i = start;
		while (line[*i])
			result[(*i)++] = 0;
		*i = start;
	}
	else if (line[*i] == '\'')
		result[(*i)] = 1;
}

void	mark_double_quote(char *line, int *result, int *i)
{
	int	start;

	start = *i;
	result[(*i)++] = 2;
	while (line[(*i)] && line[(*i)] != '\"')
		result[(*i)++] = 2;
	if (line[*i] == 0)
	{
		*i = start;
		while (line[*i])
			result[(*i)++] = 0;
		*i = start;
	}
	else if (line[*i] == '\"')
		result[(*i)] = 2;
}

int	*create_quote_info(char *line)
{
	int	*result;
	int	i;

	result = (int *)calloc(strlen(line) + 1, sizeof(int));
	if (result == 0)
		return (0);//malloc error
	i = 0;
	while (line[i])
	{
		if (line[i] == '\'')
			mark_single_quote(line, result, &i);
		else if (line[i] == '\"')
			mark_double_quote(line, result, &i);
		i++;
	}
	return (result);
}

bool	is_delim(const char *delim, char c)
{
	int	i;

	i = 0;
	while (delim[i])
	{
		if (delim[i] == c)
			return (TRUE);
		i++;
	}
	return (FALSE);
}

/*behavior would be undefined if the function reads a new string before
it finishes reading previous one.*/
char	*ft_strtok(char *str, const char *delim, int *quote_info)
{
	static int	offset;
	int			i;
	int			sublen;
	char		*result;

	while (!quote_info[offset] && is_delim(delim, str[offset]) && str[offset])
		offset++;
	if (str[offset] == '\0')
		return (offset = 0, NULL);
	sublen = 0;
	while (!(!quote_info[offset + sublen] && is_delim(delim, str[offset + sublen])) && str[offset + sublen])
		sublen++;
	result = (char *)calloc(sublen + 1, sizeof(char));
	if (result == 0)
		return ((char *)-1);//malloc error
	i = 0;
	while (i < sublen)
		result[i++] = str[offset++];
	return (result);
}

int		count_tokens(char *str, int *quote_info, const char *delim)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		while (!quote_info[i] && is_delim(delim, str[i]) && str[i])
			i++;
		if (str[i])
			count++;
		while (!(!quote_info[i] && is_delim(delim, str[i])) && str[i])
			i++;
	}
	return (count);
}

int	count_single_quote(char *line, int *i, int *quote_info)
{
	int	start;

	start = *i;
	(*i)++;
	while (line[(*i)] && line[(*i)] != '\'' && quote_info[*i] == 1)
		(*i)++;
	if (line[*i] == 0)
	{
		*i = start + 1;
		return (0);
	}
	else if (line[*i] == '\'' && quote_info[*i] == 1)
	{
		(*i)++;
		return (1);
	}
}

/* maybe quote_info is redundant here...*/
int	count_double_quote(char *line, int *i, int *quote_info)
{
	int	start;

	start = *i;
	(*i)++;
	while (line[(*i)] && line[(*i)] != '\"' && quote_info[*i] == 2)
		(*i)++;
	if (line[*i] == 0)
	{
		*i = start + 1;
		return (0);
	}
	else if (line[*i] == '\"' && quote_info[*i] == 2)
	{
		(*i)++;
		return (1);
	}
}

int		count_words(char *str, int *quote_info)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (quote_info[i] == 1)
			count += count_single_quote(str, &i, quote_info);
		else if (quote_info[i] == 2)
			count += count_double_quote(str, &i, quote_info);
		else if (str[i])
		{
			count++;
			while (str[i] && quote_info[i] == 0)
				i++;
		}
	}
	return (count);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*sub;
	size_t	i;

	if (start >= ft_strlen(s))
	{
		sub = (char *)malloc(sizeof (char) * 1);
		if (sub != 0)
			sub[0] = '\0';
		return (sub);
	}
	else if (len > ft_strlen(s + start))
		sub = (char *)malloc(sizeof (char) * (ft_strlen(s + start) + 1));
	else
		sub = (char *)malloc(sizeof (char) * (len + 1));
	if (sub == 0)
		return (NULL);
	i = 0;
	while (i < len && s[start + i] && start < ft_strlen(s))
	{
		sub[i] = s[(size_t)start + i];
		i++;
	}
	sub[i] = '\0';
	return (sub);
}

t_word	assign_single_quote(char *line, int *i, int *quote_info)
{
	int		start;
	int		size;
	t_word	result;

	result.quote_type = single_quoted;
	size = 0;
	(*i)++;
	start = *i;
	if (line[(*i)] == '\'' && quote_info[*i] == 1)
	{
		result.word = ft_substr(line, start, 0);
		return ((*i)++, result);
	}
	while (line[(*i)] && line[(*i)] != '\'' && quote_info[*i] == 1)
	{
		size++;
		(*i)++;
	}
	(*i)++;
	result.word = ft_substr(line, start, size);
	return (result);
}

t_word	assign_double_quote(char *line, int *i, int *quote_info)
{
	int		start;
	int		size;
	t_word	result;

	result.quote_type = double_quoted;
	size = 0;
	(*i)++;
	start = *i;
	if (line[(*i)] == '\"' && quote_info[*i] == 2)
	{
		result.word = ft_substr(line, start, 0);
		return ((*i)++, result);
	}
	while (line[(*i)] && line[(*i)] != '\"' && quote_info[*i] == 2)
	{
		size++;
		(*i)++;
	}
	(*i)++;
	result.word = ft_substr(line, start, size);
	return (result);
}

t_word	assign_non_quote(char *line, int *i, int *quote_info)
{
	int		start;
	int		size;
	t_word	result;

	result.quote_type = non_quoted;
	size = 0;
	start = *i;
	while (line[*i] && quote_info[*i] == 0)
	{
		(*i)++;
		size++;
	}
	result.word = ft_substr(line, start, size);
	return (result);
}

int	copy_words(char *str, t_word *result, int *quote_info)
{
	int	i;
	int	index;

	index = 0;
	i = 0;
	while (index < count_words(str, quote_info))
	{
		if (str[i] == '\'' && quote_info[i] == 1)
			result[index] = assign_single_quote(str, &i, quote_info);
		else if (str[i] == '\"' && quote_info[i] == 2)
			result[index] = assign_double_quote(str, &i, quote_info);
		else if (str[i] && quote_info[i] == 0)
			result[index] = assign_non_quote(str, &i, quote_info);
		if (result[index].word == 0)
			printf("mallocerror\n");
		index++;
	}
	result[index].word = 0;
	return (0);
}

t_word	*extract_words(char *str)
{
	int	*quote_info;
	t_word	*result;
	int		i;

	quote_info = create_quote_info(str);
	if (quote_info == 0)
		return (NULL);//malloc_error
	result = (t_word *)calloc(count_words(str, quote_info) + 1, sizeof(t_word));
	if (result == 0)
		return (NULL);//malloc_error
	if (copy_words(str, result, quote_info) == -1)
		return (NULL);//malloc_error
	return (result);
}

t_token	*extract_tokens(char *line)
{
	int		*quote_info;
	int		i;
	t_token	*result;
	char	*current_token;
	int		j;//debug


	quote_info = create_quote_info(line);
	if (quote_info == 0)
		return (NULL);// malloc error
	result = (t_token *)calloc(count_tokens(line, quote_info, " \t") + 1, sizeof (t_token));
	if (result == 0)
		return (free(quote_info), NULL);// malloc error
	i = 0;
	while (1)
	{
		current_token = ft_strtok(line, " \t", quote_info);// check malloc error
		if (current_token == 0)
			break;
/* 		printf("current_token%s\n", current_token); */
		result[i].words = extract_words(current_token);// check malloc error
		i++;
	}
	result[++i].words = 0;
	return (result);
}

void	tokenreader(t_token *tokens)
{
	int	i;
	int	j;

	i = 0;
	while (tokens[i].words)
	{
		printf("token %d\n", i);
		j = 0;
		while (tokens[i].words[j].word)
		{
			printf("\tindex\t%d\n", j);
			printf("\tword\t:%s\n", tokens[i].words[j].word);
			printf("\ttype\t:%d\n", tokens[i].words[j].quote_type);
			printf("\n");
			j++;
		}
		i++;
	}
}

int	main()
{
	t_token	*tokens;
	char	*line;
	int		*info;
	int		i;
	char	*tok;
/* 	line = "word1 word2 word3";
	info = create_quote_info(line);
	while (1)
	{
		tok = ft_strtok(line, " ", info);

		if (tok == 0)
			break ;
		printf("%s\n", tok);
	} */
	while (1)
	{
		line = readline("%");
		printf("line : %s\n", line);
		tokens = extract_tokens(line);
		printf("------token read---------\n");
		tokenreader(tokens);
	}
	return (0);
}
