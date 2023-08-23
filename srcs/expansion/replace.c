/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jihalee <jihalee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 12:22:20 by solee2            #+#    #+#             */
/*   Updated: 2023/08/17 22:54:52 by jihalee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	replace_params(t_lexeme *lexemes)
{
	t_lexeme	*current;
	char		*newstr;

	current = lexemes;
	while (current)
	{
		newstr = 0;
		if (current->exp == TRUE)
		{
			newstr = find_param(&(current->str[1]));
			if (newstr)
			{
				current->p_found = TRUE;
				free(current->str);
				current->str = newstr;
			}
		}
		current = current->next;
	}
}
