/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jihalee <jihalee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 12:21:15 by solee2            #+#    #+#             */
/*   Updated: 2023/08/29 17:11:54 by jihalee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
			&& lexemes->str[1] != '\0' \
			&& lexemes->type != 1)
			lexemes->exp = TRUE;
		lexemes = lexemes->next;
	}
}

void	replace_tokens(t_token **tokens, t_token **expanded, \
t_token **next, t_token **prev)
{
	if (*prev == 0)
		*tokens = *expanded;
	else
		(*prev)->next = *expanded;
	while ((*expanded)->next)
		(*expanded) = (*expanded)->next;
	(*expanded)->next = *next;
	*prev = *expanded;
}

int	treat_empty_exp(t_token **tokens, t_token **crnt, \
t_token **next, t_token **prev)
{
	if ((*crnt)->type == 1 || (*crnt)->type == 3 || (*crnt)->type == 4)
	{
		(*crnt)->type = amb_redir;
		*prev = *crnt;
		*crnt = *next;
		return (-1);
	}
	else if (*prev == 0)
		*tokens = *next;
	else
		(*prev)->next = *next;
	return (0);
}

void	expansion(t_token **tokens)
{
	t_token	*crnt;
	t_token	*next;
	t_token	*prev;
	t_token	*expanded;

	prev = 0;
	crnt = *tokens;
	while (crnt)
	{
		next = crnt->next;
		expanded = token_to_etoken(crnt);
		if (expanded)
			expanded->type = crnt->type;
		if (expanded)
			replace_tokens(tokens, &expanded, &next, &prev);
		else if (!expanded)
		{
			if (treat_empty_exp(tokens, &crnt, &next, &prev))
				continue ;
		}
		free((free(crnt->str), crnt));
		crnt = next;
	}
}
