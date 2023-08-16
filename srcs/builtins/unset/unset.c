#include "minishell.h"

int	unset_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while ((s1[i] || s2[i]) && i < n)
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	if (s1[i] == '=')
		return (0);
	return (1);
}

t_env	*check_first(t_env *env, char *value)
{
	t_env	*current;
	t_env	*next;

	current = env;
	if ((current && current->str) && \
		(unset_strncmp(current->str, value, get_length(value)) == 0))
	{
		next = current->next;
		free(current->str);
		free(current);
		current = next;
	}
	return (current);
}

void	ft_unset(t_job **lst)
{
	t_env	**env;
	t_env	*current;
	t_env	*next;
	t_job	*temp;
	int		i;

	env = get_env_address();
	temp = *lst;
	i = -1;
	if (!temp->arg)
		return ;
	while (temp->arg[++i])
	{
		current = check_first(*env, temp->arg[i]);
		while (current)
		{
			next = current->next;
			if (next && unset_strncmp(next->str, temp->arg[i], \
				get_length(temp->arg[i])) == 0)
			{
				current->next = current->next->next;
				free((free(next->str), next));
				break ;
			}
			current = current->next;
		}
	}
}
