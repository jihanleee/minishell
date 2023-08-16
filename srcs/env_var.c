#include "minishell.h"

t_env	**get_env_address(void)
{
	static	t_env *env;

	return (&env);
}

void	read_env(void)
{
	t_env	*current;

	current = *get_env_address();
	while (current)
	{
		printf("%s\n", current->str);
		current = current->next;
	}
}

void	clear_env(void)
{
	t_env	*current;
	t_env	*next;

	current = *get_env_address();
	while (current)
	{
		next = current->next;
		if (current->str)
			free(current->str);
		free(current);
		current = next;
	}
}

void	init_env_var(char **envp)
{
	int		i;
	t_env	**env;
	t_env	*current;

	env = get_env_address();
	if (!envp[0])
		return ;
	*env = (t_env *)ft_calloc(1, sizeof (t_env));
	(*env)->str = ft_strdup(envp[0]);
	current = (*env);
	i = 1;
	while (envp[i])
	{
		current->next = (t_env *)ft_calloc(1, sizeof (t_env));
		current->next->str = ft_strdup(envp[i]);
		current = current->next;
		i++;
	}
}
