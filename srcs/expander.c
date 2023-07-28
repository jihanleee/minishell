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

void	clear_lexemes(t_lexeme **lst, void (*del)(void *))
{
	t_lexeme	*current;
	t_lexeme	*next;

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

t_lexeme	*new_lexeme(char *str, t_quote_type type)
{
	t_lexeme *new;
	
	new = (t_lexeme *)ft_calloc(1, sizeof (t_lexeme));
	if (new == NULL)
		return (NULL);
	new->str = str;
	if (new->str == NULL)
		return (free(new), NULL);
	new->type = type;
	return (new);
}

int	append_lexeme(t_lexeme **lexemes, t_lexeme *new)
{
	t_lexeme	*current;

	if (new == 0)
		return (-1);
	if (*lexemes == 0)
		*lexemes = new;
 	else
	{
		current = *lexemes;
		while (current->next)
			current = current->next;
		current->next = new;
	}
	return (0);
}

void	read_lexemes(t_lexeme *current)
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

t_lexeme	*word_to_lexemes(char *str)
{
	int				i;
	char			*newstr;
	t_lexeme			*result;
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
		append_lexeme(&result, new_lexeme(newstr, type));
	}
	split_expansions(result);
	return (free(quote_info), result);
}

void	replace_params(t_lexeme *lexemes, char **envp)
{
	t_lexeme	*current;
	char	*newstr;

	current = lexemes;
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

int	lexemelen(t_lexeme *lexemes, t_token_type type)
{
	t_lexeme	*current;
	int		*lexeme;
	int		i;
	int		count;

	count = 0;
	current = lexemes;
	while (current)
	{
		i = 0;
		while (current->str[i])
		{
			if (current->exp == TRUE && current->p_found == FALSE && type != heredoc)
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

t_token	*new_expanded_token(int *lexeme, int start, int len)
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
		new->str[i++] = (char)lexeme[start++];
	return (new);
}

/*returns an int array in which IFS will be translated into -1,
and used as separator to perform field splitting.
if not IFS, each element is stored as ascii value of the original character.
the returned array is NULL-terminated.*/
int	*lexemes_to_int(t_lexeme *lexemes, t_token_type type)
{
	t_lexeme	*current;
	int		*lexeme;
	int		i;
	int		j;

	j = 0;
	lexeme = (int *)ft_calloc(lexemelen(lexemes, type) + 1, sizeof(int));
	if (lexeme == 0)
		return (NULL);
	current = lexemes;
	while (current)
	{
		ft_printf("lexems_to_int_while\n");
		while (current && current->exp == TRUE && current->p_found == FALSE && type != heredoc)
			current = current->next;
		if (current == 0)
			break ;
		i = 0;
		while (current->str[i])
		{
			if (is_ifs(current->str[i]) && current->type == 0)
				lexeme[j++] = -1;
			else
				lexeme[j++] = current->str[i];
			i++;
		}
		current = current->next;
	}
	return (lexeme);
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
				len = token_op_len(line, start, quote_info);new_expanded_token()
				return (clear_tokens(tokens, free), (free(quote_info), -1));
			start += len;
		}
	}
	return (0);
}
*/

t_token	*iword_to_tokens(int *lexeme)
{
	t_token	*result;
	int		start;
	int		len;

	result = 0;
	start = 0;
	while (lexeme[start])
	{
		while(lexeme[start] == -1)
			start++;
		len = 0;
		while (lexeme[start + len] && lexeme[start + len] != -1)
			len++;
		if (append_token(&result, new_expanded_token(lexeme, start, len)) == -1)
			return (clear_tokens(result, free), (free(lexeme), -1));
		start += len;
	}
	ft_printf("exiting iword_to_tokens\n");
	return (result);
}

t_token	*token_to_etoken(t_token *old, char **envp)
{
	t_lexeme	*lexemes;
	t_token		*result;
	int			*iword;
	bool		allexp;

	lexemes = word_to_lexemes(old->str);
	read_lexemes(lexemes);
	if (old->type != heredoc)
		replace_params(lexemes, envp);
	iword = lexemes_to_int(lexemes, old->type);
	allexp = TRUE;
	while (lexemes)
	{
		if (lexemes->exp == FALSE)
			allexp = FALSE;
		lexemes = lexemes->next;
	}
	result = iword_to_tokens(iword);
	if (allexp == FALSE && !result)
	{
		result = (t_token *)ft_calloc(1, sizeof (t_token));
		//protect memory alloc
		result->str = ft_strdup("");
	}
	//free lexemes
	return (result);
}

void	expansion(t_token **tokens, char **envp)
{
	t_token	*current;
	t_token	*next;
	t_token	*prev;
	t_token	*expanded;

	prev = 0;
	current = *tokens;
	while (current)
	{
		next = current->next;
		expanded = token_to_etoken(current, envp);
		read_tokens(expanded);

		if (expanded)
			expanded->type = current->type;
		if (expanded == 0 && current->type == 1 || \
			current->type == 3 || current->type == 4)
		{
			current->type = amb_redir;
			prev = current;
			current = next;
		}
		else if (prev == 0 && current->type != amb_redir)
		{
			if (expanded == NULL)
				*tokens = next;
			else
			{
				*tokens = expanded;
				while (expanded->next)
					expanded = expanded->next;
				expanded->next = next;
				prev = expanded;
			}
		}
		else if (current->type != amb_redir)
		{
			if (expanded == NULL)
				prev->next = next;
			else
			{
				prev->next = expanded;
				while (expanded->next)
					expanded = expanded->next;
				expanded->next = next;
				prev = expanded;
			}
		}
		free(current);
		current = next;
	}
}

/* int	main(int argc, char **argv, char **envp)
{
	t_token	*tokens;
	//t_token	*temp;
	char	*line;
	//int		*info;
	//int		i;
	//char	*tok;

	while (1)
	{	//temp = &tokens;
		line = readline("%");
		ft_printf("line : %s\n", line);
		tokens = create_tokens(line);
		printf("\n-----tokenize done-----\n");
		printf("\n-----parsing starts-----\n");
		if (check_tokens(tokens) != 0)
		{
			exit_error("bash: syntax error\n", &tokens);
		}
		else
		{
			tokens = parse_tokens(&tokens, free);
			printf("\ntokens after parsing\n");
			temp_read_tokens(&tokens); //
		}
		//expansion
		//execution
		expansion(&tokens, envp);
		temp_read_tokens(&tokens);
		clear_tokens(&tokens, free);
			//parsing error 있는 경우 이미 exit_error에서 clear_tokens를 함
			//main 정확히 짤 때는 두번 콜되지 않게 조심하기
	}
	return (0);
} */

/*expansion module tests*/
/* int	main(int argc, char **argv, char **envp)
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
} */