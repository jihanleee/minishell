/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jihalee <jihalee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 14:00:07 by solee2            #+#    #+#             */
/*   Updated: 2023/08/18 14:46:19 by jihalee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	validate_and_add(t_job **lst)
{
	t_job	*current;
	int		i;

	current = *lst;
	i = 0;
	while (current->arg[i])
	{
		if (first_is_valid(current->arg[i][0]) == 1)
		{
			export_error(current->arg[i]);
			if (current->arg[i + 1])
				i++;
			else
				break ;
		}
		if (middle_is_valid(current->arg[i]) == 0)
		{
			if (middle_error_case(current->arg[i]) == 0)
				add_to_env(current->arg[i]);
		}
		else
			export_error(current->arg[i]);
		i++;
	}
	return (0);
}

char	*get_key(char *str)
{
	char	*result;
	int		i;
	int		j;

	if (!str)
		return (NULL);
	i = 0;
	while (str[i])
	{
		if (str[i] == '=')
			break ;
		i++;
	}
	result = (char *)ft_calloc(i + 1, sizeof(char));
	j = 0;
	while (j < i)
	{
		result[j] = str[j];
		j++;
	}
	result[j] = '\0';
	return (result);
}

int	add_to_env(char *str)
{
	t_env	**env;
	t_env	*current;
	char	*key;

	env = get_env_address();
	if (middle_error_case(str) == 1)
		export_error(str);
	key = get_key(str);
	if (export_unset(key) == 0)
	{
		if (*env == NULL)
		{
			*env = (t_env *)ft_calloc(1, sizeof(t_env));
			(*env)->str = ft_strdup(str);
		}
		else
		{
			current = (*env);
			while (current && current->next != NULL)
				current = current->next;
			current->next = (t_env *)ft_calloc(1, sizeof (t_env));
			current->next->str = ft_strdup(str);
		}
	}
	return (free(key), 0);
}

//problems found when env is emtpy, 
int	ft_export(t_job **lst, int fd)
{
	t_job	*current;

	current = *lst;
	if (!current->arg)
	{
		export_env(fd);
		return (-1);
	}
	else if (current->arg == NULL)
	{
		export_error("");
		return (-1);
	}
	if (current->arg[0] == NULL)
		ft_env(lst, fd);
	else
		validate_and_add(lst);
	return (0);
}
