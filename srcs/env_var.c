#include "minishell.h"

t_env	**get_env_address()
{
	static t_env *env;
	return (&env);
}

void	read_env()
{
	t_env	*current;

	current = *get_env_address();
	while (current)
	{
		ft_printf("current %s\n", current->str);
		current = current->next;
	}
}

void	init_env_var(char **envp)
{
	int	i;
	t_env	**env;
	t_env	*current;

	env = get_env_address();
	*env = (t_env *)calloc(1, sizeof (t_env));
	(*env)->str = ft_strdup(envp[0]);
	current = (*env);
	i = 1;
	while (envp[i])
	{
		current->next = (t_env *)calloc(1, sizeof (t_env));
		current->next->str = ft_strdup(envp[i]);
		current = current->next;
		i++;
	}
}