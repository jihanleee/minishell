/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: solee2 <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 13:57:07 by solee2            #+#    #+#             */
/*   Updated: 2023/08/17 13:57:08 by solee2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	mark_single_quote(char *line, int *result, int *i)
{
	int	start;

	start = *i;
	result[(*i)++] = 1;
	while (line[(*i)] && line[(*i)] != '\'')
		result[(*i)++] = 1;
	if (line[*i] == 0)
	{
		*i = start;
		while (line[*i])
			result[(*i)++] = 0;
		*i = start;
	}
	else if (line[*i] == '\'')
		result[(*i)] = 1;
}

void	mark_double_quote(char *line, int *result, int *i)
{
	int	start;

	start = *i;
	result[(*i)++] = 2;
	while (line[(*i)] && line[(*i)] != '\"')
		result[(*i)++] = 2;
	if (line[*i] == 0)
	{
		*i = start;
		while (line[*i])
			result[(*i)++] = 0;
		*i = start;
	}
	else if (line[*i] == '\"')
		result[(*i)] = 2;
}

int	*create_quote_info(char *line)
{
	int	*result;
	int	i;

	result = (int *)ft_calloc(ft_strlen(line) + 1, sizeof(int));
	if (result == 0)
		return (NULL);
	i = 0;
	while (line[i])
	{
		if (line[i] == '\'')
			mark_single_quote(line, result, &i);
		else if (line[i] == '\"')
			mark_double_quote(line, result, &i);
		i++;
	}
	return (result);
}
