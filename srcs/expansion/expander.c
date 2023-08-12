#include "minishell.h"

void	chop_lexeme_str(t_lexeme **crnt, t_lexeme **next)
{
	int			i;
	t_lexeme	*new;

	i = 0;
	while ((*crnt)->type != 1 && (*crnt)->str[i])
	{
		if ((*crnt)->str[i] == '$' && (*crnt)->type != 1)
			i += len_lvname(&((*crnt)->str[i]));
		else
			while ((*crnt)->str[i] && (*crnt)->str[i] != '$')
				i++;
		if ((*crnt)->str[i])
		{
			new = new_lexeme(ft_strdup(&((*crnt)->str[i])), (*crnt)->type);
			if (new)
			{
				(*crnt)->next = new;
				(*crnt)->next->next = (*next);
				(*crnt)->str[i] = '\0';
			}
		}
	}
}

void	split_expansions(t_lexeme *lexemes)
{
	t_lexeme	*crnt;
	t_lexeme	*next;

	crnt = lexemes;
	while (crnt)
	{
		next = crnt->next;
		chop_lexeme_str(&crnt, &next);
		crnt = crnt->next;
	}
	while (lexemes)
	{
		if (lexemes->str[0] == '$' \
		&& lexemes->str[1] != '\0' && lexemes->type != 1)
			lexemes->exp = TRUE;
		lexemes = lexemes->next;
	}
}

void	substitute_tokens(t_token **exp, t_token **next, \
t_token **prev, t_token **crnt)
{
	if (*prev && *exp)
		(*prev)->next = *exp;
	if (*prev && !(*exp))
		(*prev)->next = *next;
	if (*exp)
	{
		while ((*exp)->next)
			(*exp) = (*exp)->next;
		(*exp)->next = *next;
		*prev = (*exp);
		(*exp)->type = (*crnt)->type;
	}
	if (!(*exp) && \
	((*crnt)->type == 1 || (*crnt)->type == 3 || (*crnt)->type == 4))
	{
		(*crnt)->type = amb_redir;
		(*prev) = (*crnt);
		(*crnt) = (*next);
	}
}

void	expansion(t_token **tokens, char **envp)
{
	t_token	*crnt;
	t_token	*next;
	t_token	*prev;
	t_token	*expanded;
	int		type;

	prev = 0;
	crnt = *tokens;
	while (crnt)
	{
		type = crnt->type;
		next = crnt->next;
		expanded = token_to_etoken(crnt, envp);
		if (!prev && expanded)
			*tokens = expanded;
		if (!prev && !expanded && crnt->type == 0)
			*tokens = next;
		substitute_tokens(&expanded, &next, &prev, &crnt);
		if (!expanded && (type == 1 || type == 3 || type == 4))
			continue ;
		free((free(crnt->str), crnt));
		crnt = next;
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
