#include "minishell.h"

static bool	is_allexp(t_lexeme *lexemes)
{
	bool	allexp;

	allexp = TRUE;
	while (lexemes)
	{
		if (lexemes->exp == FALSE || \
		(lexemes->type == 2 && lexemes->exp))
			allexp = FALSE;
		lexemes = lexemes->next;
	}
	return (allexp);
}

t_token	*token_to_etoken(t_token *old, char **envp)
{
	t_lexeme	*lexemes;
	t_token		*result;
	int			*iword;

	lexemes = word_to_lexemes(old->str);
	read_lexemes(lexemes);
	if (old->type != heredoc)
		replace_params(lexemes, envp);
	iword = lexemes_to_int(lexemes, old->type);
	result = iword_to_tokens(iword);
	if (!is_allexp(lexemes) && !result)
	{
		result = (t_token *)ft_calloc(1, sizeof (t_token));
		if (result)
		result->str = ft_strdup("");
	}
	clear_lexemes(&lexemes, free);
	return (free(iword), result);
}
