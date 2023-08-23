/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_unset.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jihalee <jihalee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/18 09:58:12 by jihalee           #+#    #+#             */
/*   Updated: 2023/08/23 16:34:08 by jihalee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pop_env_node(t_env **env, t_env **current, t_env **prev)
{
	if (*prev == 0)//fixed a typo
	{
		(*env) = (*current)->next;
		free((free((*current)->str), (*current)));
		(*current) = *env;
	}
	else
	{
		(*prev)->next = (*current)->next;
		free((free((*current)->str), (*current)));
		(*current) = (*prev)->next;
	}
}

int	export_unset(char *key)
{
	t_env	**env;
	t_env	*current;
	t_env	*prev;

	prev = 0;
	env = get_env_address();
	current = (*env);
	while (current)
	{
		if (unset_strncmp(current->str, key, ft_strlen(key)) == -1000)
			return (1);
		if (unset_strncmp(current->str, key, ft_strlen(key)) == 0)
		{
			pop_env_node(env, &current, &prev);
			return (0);
		}
		prev = current;
		current = current->next;
	}
	return (0);
}
