/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: solee2 <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 13:56:40 by solee2            #+#    #+#             */
/*   Updated: 2023/08/17 13:56:41 by solee2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*new_token(char *line, int start, int len, t_ttype type)
{
	t_token	*new;

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

void	set_tokentype(t_token *tokens)
{
	while (tokens)
	{
		if (ft_strncmp(tokens->str, "<", 2) == 0)
			tokens->type = in;
		else if (ft_strncmp(tokens->str, "<<", 2) == 0)
			tokens->type = heredoc;
		else if (ft_strncmp(tokens->str, ">", 2) == 0)
			tokens->type = out;
		else if (ft_strncmp(tokens->str, ">>", 2) == 0)
			tokens->type = append;
		else if (ft_strncmp(tokens->str, "|", 2) == 0)
			tokens->type = pipe_op;
		tokens = tokens->next;
	}
}

int	line_to_token(t_token **tokens, int *quote_info, char *line)
{
	int				len;
	int				start;
	t_ttype			type;

	start = 0;
	*tokens = 0;
	while (line[start])
	{
		if (is_blank(line[start]) && !quote_info[start])
			start++;
		else
		{
			if (is_op(line[start]) && !quote_info[start])
				len = token_op_len(line, start, quote_info);
			else
				len = token_word_len(line, start, &type, quote_info);
			if (append_token(tokens, new_token(line, start, len, type)) == -1)
				return (clear_tokens(tokens, free), (free(quote_info), -1));
			start += len;
		}
	}
	return (0);
}

t_token	*create_tokens(char *line)
{
	t_token			*tokens;
	int				*quote_info;

	quote_info = create_quote_info(line);
	if (quote_info == 0)
		return (NULL);
	if (line_to_token(&tokens, quote_info, line) == -1)
		return (NULL);
	set_tokentype(tokens);
	return (free(quote_info), tokens);
}
