/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clear.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: solee2 <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 12:16:52 by solee2            #+#    #+#             */
/*   Updated: 2023/08/17 12:16:55 by solee2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

void	clear_jobs(t_job *current)
{
	t_job	*next;

	if (current == 0)
		return ;
	while (current->prev)
		current = current->prev;
	while (current)
	{
		next = current->next;
		if (current->cmd)
			free(current->cmd);
		if (current->outfile)
			free(current->outfile);
		if (current->infile)
			free(current->infile);
		if (current->arg)
			free_arrays(current->arg);
		free(current);
		current = next;
	}
}
