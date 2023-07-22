#include "minishell.h"

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
		return (-1);//malloc error
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

int	count_single_quote(char *line, int *i)
{
	int	start;

	start = *i;
	(*i)++;
	while (line[(*i)] && line[(*i)] != '\'')
		(*i)++;
	(*i)++;
	return (1);
}

int	count_double_quote(char *line, int *i)
{
	int	start;

	start = *i;
	(*i)++;
	while (line[(*i)] && line[(*i)] != '\"')
		(*i)++;
	(*i)++;
	return (1);
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
			count += count_single_quote(str, &i);
		else if (quote_info[i] == 2)
			count += count_double_quote(str, &i);
		else if (str[i])
		{
			count++;
			while (str[i] && quote_info[i] == 0)
				i++;
		}
	}
	return (count);
}

/* t_word	assign_single_quote(char *line, int *i, int *quote_info)
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
} */

bool	is_op(char c)
{
	if (c == '>' || c == '<' || c == '|')
		return (1);
	else
		return (0);
}

bool	is_blank(char c)
{
	if (c == ' ' || c == '\t')
		return (1);
	else
		return (0);
}

t_token	*create_tokens(char *line, int *quote_info)
{
	t_token			*tokens;
	int				start;
	int				len;
	t_token_type	type;
	char			*str;

 	tokens = 0;
	start = 0;
	while (line[start])
	{
		len = 0;
		if (is_blank(line[start]) && !quote_info[start])
			start++;
		else
		{
			if (is_op(line[start]) && !quote_info[start])
			{
				if (line[start] == '<' && !quote_info[start])
				{
					if (line[start + 1] && !quote_info[start + 1])
						len = 2;
					else
						len = 1;
				}
				else if (line[start] == '>')
				{
					if (line[start + 1] && !quote_info[start + 1])
						len = 2;
					else
						len = 1;
				}
				else if (line[start] == '|')
					len = 1;
				type = operator;
			}
			else
			{
				while (!((is_op(line[start + len])
						|| is_blank(line[start + len]))
							&& !quote_info[start + len])
								&& line[start + len])
					len++;
			}
			ft_printf("s %d, len %d \n", start, len);
			str = ft_substr(line, start, len);
			printf("%s\n", str);
			start += len;
		}
	}
	return (NULL);
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
		info = create_quote_info(line);
		create_tokens(line, info);
	}
	return (0);
}
