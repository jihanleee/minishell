/*this file is useless for now, some functions can be useful in future!*/

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
