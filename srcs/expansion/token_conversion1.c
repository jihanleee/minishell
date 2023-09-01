/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_conversion1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jihalee <jihalee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 12:22:32 by solee2            #+#    #+#             */
/*   Updated: 2023/08/29 15:45:31 by jihalee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*new_expanded_token(int *lexeme, int start, int len)
{
	t_token	*new;
	int		i;

	new = (t_token *)ft_calloc(1, sizeof (t_token));
	if (new == NULL)
		return (NULL);
	if (len)
		new->str = ft_calloc(sizeof(char), len + 1);
	else
		return (free(new), NULL);
	if (new->str == NULL)
		return (free(new), NULL);
	i = 0;
	while (i < len)
		new->str[i++] = (char)lexeme[start++];
	return (new);
}

void	copy_lexemes_to_int(int	*lexeme, t_lexeme *current, int *i, int *j)
{
	*i = 0;
	while (current->str[*i])
	{
		if (is_ifs(current->str[*i]) && current->type == 0)
			lexeme[(*j)++] = -1;
		else
			lexeme[(*j)++] = current->str[*i];
		(*i)++;
	}
}

/*returns an int array in which IFS will be translated into -1,
and used as separator to perform field splitting.
if not IFS, each element is stored as ascii value of the original character.
the returned array is NULL-terminated.*/
int	*lexemes_to_int(t_lexeme *lexemes, t_ttype type)
{
	t_lexeme	*current;
	int			*lexeme;
	int			i;
	int			j;

	j = 0;
	lexeme = (int *)ft_calloc(lexemelen(lexemes, type) + 1, sizeof(int));
	current = lexemes;
	while (current && lexeme)
	{
		while (current && current->exp == TRUE \
		&& current->p_found == FALSE && type != heredoc)
			current = current->next;
		if (current == 0)
			break ;
		copy_lexemes_to_int(lexeme, current, &i, &j);
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
		while (lexeme[start] == -1)
			start++;
		len = 0;
		while (lexeme[start + len] && lexeme[start + len] != -1)
			len++;
		if (append_token(&result, new_expanded_token(lexeme, start, len)) == -1)
			return (result);
		start += len;
	}
	return (result);
}
