#include "minishell.h"

int	unset_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	//printf("size of comparison: %d\n", n);
	while ((s1[i] || s2[i]) && i < n)
	{
		//printf("comparing %c and %c\n", s1[i], s2[i]);
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	if (s1[i] == '=')
		return (0);
	return (1);
}

void	ft_unset(t_job **lst, int fd)
{
	t_env	**env;
	t_env	*current;
	t_env	*next;
	t_job	*temp;
	int		i;
	//int		length;

	env = get_env_address();
	//current = (*env);
	temp = *lst;
	i = 0;
	while (temp->arg[i])
	{
		current = (*env);
		if (current && current->str)
		{
			//length = get_length(temp->cmd);
			if (current && unset_strncmp(current->str, temp->arg[i], get_length(temp->arg[i])) == 0)
			{
				next = current->next;
				printf("\tdeleting/freeing %s\n", current->str);
				//free(current->str);
				//free(current->next);
				//free(current); //
				current = next;
			}
		}
		while (current)
		{
			next = current->next;
			//printf("%s\n", current->str);
			//length = get_length(temp->arg[0]);
			//printf("\tcurrent env line: %s\n", current->str);
			if (next && unset_strncmp(next->str, temp->arg[i], get_length(temp->arg[i])) == 0)
			{
				//printf("found it:\n\tcurrnet line is\n\t\t\t%s\n", next->str);
				current->next = current->next->next;
				printf("\tdeleting/freeing %s\n", next->str);
				//free(next->str);
				//free(next->next);
				//free(next);
				break ;
			}
			current = current->next;
		}
		i++;
	}
}
