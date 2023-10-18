/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_unset.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: solee2 <solee2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/18 09:58:12 by jihalee           #+#    #+#             */
/*   Updated: 2023/10/18 14:02:05 by solee2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pop_env_node(t_env **env, t_env **current, t_env **prev)
{
	if (*prev == 0)
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
