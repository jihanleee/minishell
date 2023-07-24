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
/*
creates quote info in a malloc'd int array. returns -1 for malloc failure.
ex)
00000011111100000222222
sgkfgj'sgjs'fskgj"sfdf"
*/
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

int	token_op_len(char *line, int start, t_token_type *type, int *quote_info)
{
	int	len;

	len = 0;
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
	*type = operator;
	return (len);
}

int	token_word_len(char *line, int start, t_token_type *type, int *quote_info)
{
	int	len;

	len = 0;
	while (!((is_op(line[start + len])
			|| is_blank(line[start + len]))
				&& !quote_info[start + len])
					&& line[start + len])
		len++;
	*type = token;
	return (len);
}

void	read_tokens(t_token *current)
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

void	clear_tokens(t_token **lst, void (*del)(void *))
{
	t_token	*current;
	t_token	*next;

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

t_token	*new_token(char *line, int start, int len, t_token_type type)
{
	t_token *new;
	
	new = (t_token *)ft_calloc(1, sizeof (t_token));
	if (new == NULL)
		return (NULL);
	new->str = ft_substr(line, start, len);
	if (new->str == NULL)
		return (free(new), NULL);
	new->type = type;
	return (new);
}

int	append_token(t_token **tokens, t_token *new)
{
	t_token	*current;

	if (new == 0)
		return (-1);
	if (*tokens == 0)
		*tokens = new;
 	else
	{
		current = *tokens;
		while (current->next)
			current = current->next;
		current->next = new;
	}
	return (0);
}

/*breaks up a line into tokens ,
assigns them to a t_token type linked-list,
and returns it. NULL is returned for malloc failure.*/
t_token	*create_tokens(char *line)
{
	t_token			*tokens;
	int				start;
	int				len;
	t_token_type	type;
	int				*quote_info;

	if ((quote_info = create_quote_info(line), quote_info) == 0)
		return (NULL);
 	tokens = (start = 0, 0);
	while (line[start])
	{
		if (is_blank(line[start]) && !quote_info[start])
			start++;
		else
		{
			if (is_op(line[start]) && !quote_info[start])
				len = token_op_len(line, start, &type, quote_info);
			else
				len = token_word_len(line, start, &type, quote_info);
			if (append_token(&tokens, new_token(line, start, len, type)) == -1)
				return (clear_tokens(&tokens, free), (free(quote_info), NULL));
			start += len;
		}
	}
	read_tokens(tokens);
	return (free(quote_info), tokens);
}

int	main(int argc, char **argv, char **envp)
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
		create_tokens(line);
	}
	return (0);
}