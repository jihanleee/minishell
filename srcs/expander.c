#include "minishell.h"

void	clear_tokens(t_token **lst, void (*del)(void *))
{
	t_token	*crnt;
	t_token	*next;

	if (lst == 0)
		return ;
	if (del == 0)
		return ;
	crnt = *lst;
	while (crnt)
	{
		next = crnt->next;
		if (crnt->str)
			del(crnt->str);
		free(crnt);
		crnt = next;
	}
	*lst = 0;
}

char	*assign_single_quote(char *line, int *i, int *quote_info, t_qtype *type)
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

char	*assign_double_quote(char *line, int *i, int *quote_info, t_qtype *type)
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

char	*assign_non_quote(char *line, int *i, int *quote_info, t_qtype *type)
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
	t_lexeme	*crnt;
	t_lexeme	*next;

	if (lst == 0)
		return ;
	if (del == 0)
		return ;
	crnt = *lst;
	while (crnt)
	{
		next = crnt->next;
		if (crnt->str)
			del(crnt->str);
		free(crnt);
		crnt = next;
	}
	*lst = 0;
}

t_lexeme	*new_lexeme(char *str, t_qtype type)
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
	t_lexeme	*crnt;

	if (new == 0)
		return (-1);
	if (*lexemes == 0)
		*lexemes = new;
 	else
	{
		crnt = *lexemes;
		while (crnt->next)
			crnt = crnt->next;
		crnt->next = new;
	}
	return (0);
}

void	read_lexemes(t_lexeme *crnt)
{
	int		i;
	
	i = 0;
	while (crnt)
	{
		ft_printf("\tindex:\t%d\n", i);
		ft_printf("\tstr:\t%s\n", crnt->str);
		ft_printf("\ttype:\t%d\n", crnt->type);
		if (crnt->exp == 1)
			ft_printf("\texp:\tTRUE\n");
		else
			ft_printf("\texp:\tFALSE\n");
		ft_printf("p_found %d\n", crnt->p_found);
		crnt = crnt->next;
		i++;
	}
}

int    len_lvname(char *str)
{
    int    i;

    if (ft_isdigit(str[1]) || str[1] == '?')
        return (2);
    i = 1;
    while (ft_isalnum(str[i]) || str[i] == '_')
        i++;
    return (i);
}

void    split_expansions(t_lexeme *lexemes)
{
    t_lexeme    *crnt;
    t_lexeme    *next;
    int            i;

    crnt = lexemes;
    while (crnt)
    {
        next = crnt->next;
        //split them!
        i = 0;
        while (crnt->type != 1 && crnt->str[i])
        {
            if (crnt->str[i] == '$' && crnt->type != 1)
                i += len_lvname(&(crnt->str[i]));
            else
                while (crnt->str[i] && crnt->str[i] != '$')
                    i++;
            if (crnt->str[i])
            {
                crnt->next = new_lexeme(ft_strdup(&(crnt->str[i])), crnt->type);
                //protect the malloc
                crnt->next->next = next;
                crnt->str[i] = '\0';
            }
        }
        crnt = crnt->next;
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
	t_qtype	type;
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
	t_lexeme	*crnt;
	char	*newstr;

	crnt = lexemes;
	while (crnt)
	{
		ft_printf("%s\n", crnt->str);
		newstr = 0;
		if (crnt->exp == TRUE)
		{
			newstr = find_param(&(crnt->str[1]), envp);
			if (newstr)
			{
				crnt->p_found = TRUE;
				free(crnt->str);
				crnt->str = newstr;
			}
		}
		crnt = crnt->next;
	}
}

int	lexemelen(t_lexeme *lexemes, t_ttype type)
{
	t_lexeme	*crnt;
	//int		*lexeme;
	int		i;
	int		count;

	count = 0;
	crnt = lexemes;
	while (crnt)
	{
		i = 0;
		while (crnt->str[i])
		{
			if (crnt->exp == TRUE && crnt->p_found == FALSE && type != heredoc)
				break;
			i++;
			count++;
		}
		crnt = crnt->next;
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
int	*lexemes_to_int(t_lexeme *lexemes, t_ttype type)
{
	t_lexeme	*crnt;
	int		*lexeme;
	int		i;
	int		j;

	j = 0;
	lexeme = (int *)ft_calloc(lexemelen(lexemes, type) + 1, sizeof(int));
	if (lexeme == 0)
		return (NULL);
	crnt = lexemes;
	while (crnt)
	{
		//printf("\tlexems_to_int_while\n");
		while (crnt && crnt->exp == TRUE && crnt->p_found == FALSE && type != heredoc)
			crnt = crnt->next;
		if (crnt == 0)
			break ;
		i = 0;
		while (crnt->str[i])
		{
			if (is_ifs(crnt->str[i]) && crnt->type == 0)
				lexeme[j++] = -1;
			else
				lexeme[j++] = crnt->str[i];
			i++;
		}
		crnt = crnt->next;
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
			return (clear_tokens(&result, free), (free(lexeme), NULL));
		start += len;
	}
	//printf("\texiting iword_to_tokens\n");
	return (result);
}

t_token	*token_to_etoken(t_token *old, char **envp)
{
	t_lexeme	*lexemes;
	t_lexeme	*first;
	t_token		*result;
	int			*iword;
	bool		allexp;

	lexemes = word_to_lexemes(old->str);
	first = lexemes;
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
	clear_lexemes(&first, free);
	return (free(iword), result);
}

void	expansion(t_token **tokens, char **envp)
{
	t_token	*crnt;
	t_token	*next;
	t_token	*prev;
	t_token	*expanded;
	int		i;

	i = 0;
	prev = 0;
	crnt = *tokens;
	while (crnt)
	{
		printf("\tindex %d\n", i++);
		next = crnt->next;
		expanded = token_to_etoken(crnt, envp);
		if (expanded)
		{
			expanded->type = crnt->type;
			if (prev == 0)
				*tokens = expanded;
			else
				prev->next = expanded;
			while (expanded->next)
				expanded = expanded->next;
			expanded->next = next;
			prev = expanded;
		}
		else if (!expanded)
		{
			if (crnt->type == 1 || crnt->type == 3 || crnt->type == 4)
			{
				crnt->type = amb_redir;
				prev = crnt;
				crnt = next;
				continue ;
			}
			else if (prev == 0)
				*tokens = next;
			else
				prev->next = next;
		}
		free((free(crnt->str), crnt));
		crnt = next;
	}
}

void	read_jobs(t_job *jobs)
{
	int	i;

	while (jobs)
	{
		printf("--------------------\n");
		printf("in read jobs function, expander\n");
		//ft_printf("\tcrnt pipe has command\n", jobs->cmd);
		ft_printf("\tintype\t%d\n", jobs->in);
		ft_printf("\touttype\t%d\n", jobs->out);
		if (jobs->cmd)
			ft_printf("\tcmd\t%s\n", jobs->cmd);
		i = 0;
		if (jobs->arg)
		{
			while (jobs->arg[i])
			{
				ft_printf("\targ[%d]\t%s\n", i, jobs->arg[i]);
				i++;
			}
		}
		//printf("before in/outfile\n");
		if (jobs->infile)
			ft_printf("\tinfile\t%s\n", jobs->infile);
		//printf("after infile + before outfile\n");
		if (jobs->outfile)
		{
			//printf("inside outfile\n");
			ft_printf("\toutfile\t%s\n", jobs->outfile);
		}
		ft_printf("--------------------\n");
		if (jobs->next != NULL)
			jobs = jobs->next;
		else
			return ;
	}
}

char	**extract_arg(t_token **tokens)
{
	char		**result;
	int			size_result;
	t_token		*crnt;
	int			i;

	size_result = 0;
	crnt = *tokens;
	while (crnt && crnt->type != pipe_op)
	{
		if (crnt->type == word)
			size_result++;
		crnt = crnt->next;
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

void	create_heredoc(const char *delim)
{
	int	fd;
	char	*line;

	fd = open("heredoc.tmp", O_RDWR | O_CREAT | O_TRUNC, 0666);
	while (1)
	{
		ft_printf(">");
		line = get_next_line(0);
		if (line == 0 || line[ft_strlen(line) - 1] != '\n')
		{
			if (line)
				free(line);
			ft_putstr_fd("warning: here-doc delimited by eof\n", 2);//error
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
	t_token	*crnt;
	int		fd;

	crnt = token;
	while (crnt)
	{
		fd = 0;
		if (crnt->type == in)
			fd = open(crnt->str, O_RDONLY);
		else if (crnt->type == out || crnt->type == append)
			fd = open(crnt->str, O_WRONLY | O_CREAT | \
			O_TRUNC * (crnt->type != append), 0777);
		else if (crnt->type == heredoc)
		{
			create_heredoc(crnt->str);
			free(crnt->str);
			crnt->str = ft_strdup("heredoc.tmp");
		}
		else if (crnt->type == amb_redir)
			perror("ambiguous redirection");
		if (fd == -1)
			perror(crnt->str);
		if (fd == -1 || crnt->type == amb_redir)
		{
			while (crnt && crnt->type != pipe_op)
			{
				crnt->type = amb_redir;
				crnt = crnt->next;
			}
		}
		if (crnt)
			crnt = crnt->next;
	}
}

void	point_prev_job(t_job *jobs)
{
	while (jobs->next)
	{
		jobs->next->prev = jobs;
		jobs = jobs->next;
	}
}

t_job	*extract_jobs(t_token *tokens)
{
	t_job	*result;
	t_job	*cur_result;
	t_token	*crnt;

	if (tokens == NULL)
		return (NULL);
	result = (t_job *)ft_calloc(1, sizeof(t_job));
	cur_result = result;
	crnt = tokens;
	while (crnt)
	{
		if (crnt->type == pipe_op)
		{
			cur_result->next = (t_job *)ft_calloc(1, sizeof (t_job));
			cur_result = cur_result->next;
		}
		else if (crnt->type == in || crnt->type == heredoc)
		{
			if (cur_result->infile)
				free(cur_result->infile);
			cur_result->infile = ft_strdup(crnt->str);
			cur_result->in = in;
		}
		else if (crnt->type == out || crnt->type == append)
		{
			if (cur_result->outfile)
				free(cur_result->outfile);
			cur_result->outfile = ft_strdup(crnt->str);
			cur_result->out = crnt->type;
		}
		if (crnt->type == amb_redir)
		{
			cur_result->in = -1;
			cur_result->out = -1;
			while (crnt->next && crnt->next->type != pipe_op)
				crnt = crnt->next;
		}
		crnt = crnt->next;
	}
	crnt = tokens;
	cur_result = result;
	int	i = 0;
	while (crnt && cur_result)
	{
		while (crnt && crnt->type != pipe_op && crnt->type != word)
			crnt = crnt->next;
		if (crnt == 0)
			break ;
		if (crnt->type == pipe_op)
		{
			ft_printf("index %d\n", i++);
			cur_result = cur_result->next;
			crnt = crnt->next;
		}
		else if (crnt->type == word)
		{
			if (cur_result->cmd == 0)
			{
				//ft_printf("str:%s\n", crnt->str);
				cur_result->cmd = ft_strdup(crnt->str);
				crnt = crnt->next;
			}
			else
				cur_result->arg = extract_arg(&crnt);
		}
	}
	point_prev_job(result);
	return (result);
}

void	clear_jobs(t_job **lst)
{
	t_job	*crnt;
	t_job	*next;
	int		i;

	if (lst == 0)
		return ;
	crnt = *lst;
	while (crnt)
	{
		next = crnt->next;
		if(crnt->cmd)
			free(crnt->cmd);
		if (crnt->outfile)
			free(crnt->outfile);
		if (crnt->arg)
		{
			i = 0;
			while (crnt->arg[i])
				free(crnt->arg[i++]);
			free(crnt->arg);
		}
		free(crnt);
		crnt = next;
	}
	*lst = 0;
}
