#include "minishell.h"

static void	export_unset(char *key);

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
	int	i;
	int	j;

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

	if (middle_error_case(str) == 1)
		export_error(str);
	key = get_key(str);
	export_unset(key);
	free(key);
	env = get_env_address();
	if (*env == NULL)
	{
		*env = (t_env *)ft_calloc(1, sizeof(t_env));
		(*env)->str = ft_strdup(str);
	}
	current = (*env);
	while (current && current->next != NULL)
		current = current->next;
	current->next = (t_env *)ft_calloc(1, sizeof (t_env));
	current->next->str = ft_strdup(str);
	return (0);
}

static void	export_unset(char *key)
{
	t_env	**env;
	t_env	*current;
	t_env	*next;

	env = get_env_address();
	current = (*env);
	if (current && current->str)
	{
		if (current && unset_strncmp(current->str, key, get_length(key)) == 0)
		{
			next = current->next;
			current = next;
		}
	}
	while (current)
	{
		next = current->next;
		if (next && unset_strncmp(next->str, key, get_length(key)) == 0)
		{
			current->next = current->next->next;
			break ;
		}
		current = next;
	}
}

int	ft_export(t_job **lst, int fd)
{
	t_job	*current;

	current = *lst;
	if (!current->arg)
		return (-1);
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
