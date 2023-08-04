#include "minishell.h"

void	clear_tokens(t_token **lst, void (*del)(void *))
{
	t_token	*current;
	t_token	*next;

	if (lst == 0)
		return ;
	if (del == 0)
		return ;
	current = *lst;
	printf("\n-----------------------------------\n");
	printf("currently in clear tokens function\n"); //
	while (current)
	{
		printf("\ttoken we are deleting is %s\n", current->str);//
		next = current->next;
		if (current->str)
			del(current->str);
		free(current);
		current = next;
	}
	*lst = 0;
}

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
		ft_printf("\tindex:\t%d\n", i);
		ft_printf("\tstr:\t%s\n", current->str);
		ft_printf("\ttype:\t%d\n", current->type);
		if (current->exp == 1)
			ft_printf("\texp:\tTRUE\n");
		else
			ft_printf("\texp:\tFALSE\n");
		current = current->next;
		i++;
	}
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
*/int    len_lvname(char *str)
{
    int    i;

    if (ft_isdigit(str[1]))
        return (2);
    i = 1;
    while (ft_isalnum(str[i]) || str[i] == '_')
        i++;
    return (i);
}

void    split_expansions(t_lexeme *lexemes)
{
    t_lexeme    *current;
    t_lexeme    *next;
    int            i;
    int            len;

    current = lexemes;
    while (current)
    {
        next = current->next;
        //split them!
        i = 0;
        while (current->type != 1 && current->str[i])
        {
            if (current->str[i] == '$' && current->type != 1)
                i += len_lvname(&(current->str[i]));
            else
                while (current->str[i] && current->str[i] != '$')
                    i++;
            if (current->str[i])
            {
                current->next = new_lexeme(ft_strdup(&(current->str[i])), current->type);
                //protect the malloc
                current->next->next = next;
                current->str[i] = '\0';
            }
        }
        current = current->next;
    }
    while (lexemes)
    {
        if (lexemes->str[0] == '$' && lexemes->str[1] != '\0' && lexemes->type != 1)
            lexemes->exp = TRUE;
        lexemes = lexemes->next;
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
	//int		*lexeme;
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
		//printf("\tlexems_to_int_while\n");
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
			//return (clear_tokens(result, free), (free(lexeme), -1));
			return (clear_tokens(result, free), (free(lexeme), NULL));
		start += len;
	}
	//printf("\texiting iword_to_tokens\n");
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
	int		i;

	i = 0;
	prev = 0;
	current = *tokens;
	while (current)
	{
		printf("\tindex %d\n", i++);
		next = current->next;
		expanded = token_to_etoken(current, envp);
		if (expanded)
			expanded->type = current->type;
		if (expanded == NULL && (current->type == 1 || \
			current->type == 3 || current->type == 4))
		{
			current->type = amb_redir;
			prev = current;
			current = next;
			continue ;
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

void	read_pipes(t_pipe *pipes)
{
	int	i;

	while (pipes)
	{
		printf("--------------------\n");
		printf("in read pipes function, expander\n");
		//ft_printf("\tcurrent pipe has command\n", pipes->cmd);
		ft_printf("\tintype\t%d\n", pipes->in);
		ft_printf("\touttype\t%d\n", pipes->out);
		if (pipes->cmd)
			ft_printf("\tcmd\t%s\n", pipes->cmd);
		i = 0;
		if (pipes->arg)
		{
			while (pipes->arg[i])
			{
				ft_printf("\targ[%d]\t%s\n", i, pipes->arg[i]);
				i++;
			}
		}
		//printf("before in/outfile\n");
		if (pipes->infile)
			ft_printf("\tinfile\t%s\n", pipes->infile);
		//printf("after infile + before outfile\n");
		if (pipes->outfile)
		{
			//printf("inside outfile\n");
			ft_printf("\toutfile\t%s\n", pipes->outfile);
		}
		ft_printf("--------------------\n");
		if (pipes->next != NULL)
			pipes = pipes->next;
		else
			return ;
	}
}

char	**extract_arg(t_token **tokens)
{
	char		**result;
	int			size_result;
	t_token		*current;
	int			i;

	size_result = 0;
	current = *tokens;
	while (current && current->type != pipe_op)
	{
		if (current->type == word)
			size_result++;
		current = current->next;
	}
	result = (char **)ft_calloc(size_result + 1, sizeof (char *));
	if (result == NULL)
		return (NULL);
	i = 0;
	while (*tokens && (*tokens)->type != pipe_op)
	{
		if ((*tokens)->type == word)
			result[i++] = ft_strdup((*tokens)->str);
		*tokens = (*tokens)->next;
	}
	result[i] = 0;
	return (result);
}

void	create_heredoc(const char *delim, bool is_quoted)
{
	int	fd;
	char	*line;

	fd = open("heredoc.tmp", O_RDWR | O_CREAT | O_TRUNC, 0777);
	while (1)
	{
		ft_printf("heredoc>");
		line = get_next_line(0);
		if (line == 0 || line[ft_strlen(line) - 1] != '\n')
		{
			if (line)
				free(line);
			ft_printf("ERRRRORROR\n");//error
			break ;
		}
		else if (ft_strncmp(line, delim, ft_strlen(line) - 1) == 0 \
		&& (ft_strlen(line) - 1 == ft_strlen(delim)))
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		free(line);
	}
	close(fd);
}

void	open_file_redir(t_token *token)
{
	t_token	*current;
	int		fd;

	current = token;
	while (current)
	{
		fd = 0;
		if (current->type == in)
			fd = open(current->str, O_RDONLY);
		else if (current->type == out || current->type == append)
			fd = open(current->str, O_WRONLY | O_CREAT | \
			O_TRUNC * (current->type != append), 0777);
		else if (current->type == heredoc)
		{
			create_heredoc(current->str, 1);
			free(current->str);
			current->str = ft_strdup("heredoc.tmp");
		}
		else if (current->type == amb_redir)
			perror("ambiguous redirection");
		if (fd == -1)
			perror(current->str);
		if (fd == -1 || current->type == amb_redir)
		{
			while (current && current->type != pipe_op)
			{
				current->type = amb_redir;
				current = current->next;
			}
		}
		if (current)
			current = current->next;
	}
}

t_pipe	*extract_pipes(t_token *tokens)
{
	t_pipe	*result;
	t_pipe	*cur_result;
	t_token	*current;
	bool	cmd_found;

	if (tokens == NULL)
		return (NULL);
	result = (t_pipe *)calloc(1, sizeof(t_pipe));
	cur_result = result;
	current = tokens;
	while (current)
	{
		if (current->type == pipe_op)
		{
			cur_result->next = (t_pipe *)ft_calloc(1, sizeof (t_pipe));
			cur_result = cur_result->next;
		}
		else if (current->type == in || current->type == heredoc)
		{
			if (cur_result->infile)
				free(cur_result->infile);
			cur_result->infile = ft_strdup(current->str);
			cur_result->in = in;
		}
		else if (current->type == out || current->type == append)
		{
			if (cur_result->outfile)
				free(cur_result->outfile);
			cur_result->outfile = ft_strdup(current->str);
			cur_result->out = current->type;
		}
		if (current->type == amb_redir)
		{
			cur_result->in = -1;
			cur_result->out = -1;
			while (current->next && current->next->type != pipe_op)
				current = current->next;
		}
		current = current->next;
	}
	current = tokens;
	cur_result = result;
	int	i = 0;
	while (current && cur_result)
	{
		while (current && current->type != pipe_op && current->type != word)
			current = current->next;
		if (current == 0)
			break ;
		if (current->type == pipe_op)
		{
			ft_printf("index %d\n", i++);
			cur_result = cur_result->next;
			current = current->next;
		}
		else if (current->type == word)
		{
			if (cur_result->cmd == 0)
			{
				ft_printf("str:%s\n", current->str);
				cur_result->cmd = ft_strdup(current->str);
				current = current->next;
			}
			else
				cur_result->arg = extract_arg(&current);
		}
	}
	return (result);
}

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