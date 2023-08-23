/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jihalee <jihalee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 13:58:19 by solee2            #+#    #+#             */
/*   Updated: 2023/08/23 16:34:08 by jihalee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	change_type(t_token *current, t_token *next)
{
	if (next->type != 0 && next->type != 5)
	{
		next->next->type = next->type;
		current->next = next->next;
		next->next = NULL;
		free(next->str);
		free(next);
	}
}

t_token	*parse_tokens(t_token **lst, void (*del)(void *))
{
	t_token	*current;
	t_token	*result;
	t_token	*temp;
	t_token	*next;

	if (lst == 0 || del == 0 || *lst == 0)
		return (NULL);
	result = *lst;
	current = *lst;
	if (current->type != 0 && current->next)
	{
		temp = current;
		current->next->type = current->type;
		current = current->next;
		result = result->next;
		temp->next = NULL;
		free((free(temp->str), temp));
	}
	while (current && current->next)
	{
		next = current->next;
		change_type(current, next);
		current = current->next;
	}
	return (result);
}

int	token_error_check(t_token *tokens)
{
	if (tokens->type == 5 && tokens->next && tokens->next->type == 5)
	{
		print_parse_error(&tokens);
		return (1);
	}
	if (tokens->type != 0 && tokens->type != 5)
	{
		if (tokens->next == NULL || tokens->next->type != 0)
		{
			print_parse_error(&tokens);
			return (1);
		}
	}
	if (tokens->type == pipe_op && tokens->next && \
		tokens->next->type == pipe_op)
	{
		print_parse_error(&tokens);
		return (1);
	}
	return (0);
}

int	check_tokens(t_token *tokens)
{
	if (!tokens)
		return (0);
	if (tokens->type == 5)
	{
		print_parse_error(&tokens);
		return (1);
	}
	while (tokens && tokens->str)
	{
		if (token_error_check(tokens))
			return (1);
		tokens = tokens->next;
	}
	return (0);
}

void	print_parse_error(t_token **input)
{
	t_token	*current;

	g_exit_stat = 2;
	current = *input;
	write(2, "bash: syntax error near unexpected token \'", 42);
	if (current->type >= 1 && current->type <= 4)
	{
		if (current->next)
			write(2, current->next->str, ft_strlen(current->next->str));
	}
	else
		write(2, current->str, ft_strlen(current->str));
	write(2, "\'", 1);
	write(2, "\n", 1);
}
