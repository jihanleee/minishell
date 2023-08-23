/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jobs.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: solee2 <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 12:22:02 by solee2            #+#    #+#             */
/*   Updated: 2023/08/17 12:22:05 by solee2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	point_prev_job(t_job *jobs)
{
	while (jobs->next)
	{
		jobs->next->prev = jobs;
		jobs = jobs->next;
	}
}

int	assign_iofile(t_token **crnt, t_job **cur_result)
{
	if ((*crnt)->type == pipe_op)
	{
		(*cur_result)->next = (t_job *)ft_calloc(1, sizeof (t_job));
		if (!(*cur_result)->next)
			return (-1);
		(*cur_result) = (*cur_result)->next;
	}
	if ((*crnt)->type == in || (*crnt)->type == heredoc)
	{
		if ((*cur_result)->infile)
			free((*cur_result)->infile);
		(*cur_result)->infile = ft_strdup((*crnt)->str);
		(*cur_result)->in = in;
	}
	if ((*crnt)->type == out || (*crnt)->type == append)
	{
		if ((*cur_result)->outfile)
			free((*cur_result)->outfile);
		(*cur_result)->outfile = ft_strdup((*crnt)->str);
		(*cur_result)->out = (*crnt)->type;
	}
	return (0);
}

t_job	*extract_jobs(t_token *tokens)
{
	t_job	*result;
	t_job	*cur_result;
	t_token	*current;

	if (tokens == 0)
		return (0);
	result = (t_job *)ft_calloc(1, sizeof(t_job));
	cur_result = result;
	current = tokens;
	while (current)
	{
		if (assign_iofile(&current, &cur_result))
			break ;
		if (current->type == amb_redir)
		{
			cur_result->in = -1;
			cur_result->out = -1;
			while (current->next && current->next->type != pipe_op)
				current = current->next;
		}
		current = current->next;
	}
	assign_cmd_arg(tokens, result);
	point_prev_job(result);
	return (result);
}

void	assign_cmd_arg(t_token *current, t_job *cur_result)
{
	while (current && cur_result)
	{
		while (current && current->type != pipe_op && current->type != word)
			current = current->next;
		if (current == 0)
			break ;
		if (current->type == pipe_op)
		{
			cur_result = cur_result->next;
			current = current->next;
		}
		else if (current->type == word)
		{
			if (cur_result->cmd == 0)
			{
				cur_result->cmd = ft_strdup(current->str);
				current = current->next;
			}
			else
				cur_result->arg = extract_arg(&current);
		}
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
