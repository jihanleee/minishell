#include "minishell.h"

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
	read_lexemes(lexemes);
	if (old->type != heredoc)
		replace_params(lexemes, envp);
	iword = lexemes_to_int(lexemes, old->type);
	allexp = TRUE;
	while (lexemes)
	{
		if (lexemes->exp == FALSE || \
		(lexemes->type == 2 && lexemes->exp))
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
