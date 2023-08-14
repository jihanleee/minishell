#include "minishell.h"

/*
static void	delete_line(char **env, char *string)
{
	int i;
	int length;

	i = 0;
	length = get_length(string);
	while (env[i])
	{
		if (ft_strncmp(env[i], string, length) == 0)
			break ;
		i++;
	}
	env[i] = NULL;
	free(env[i]);
}
*/

/*
int	check_middle_values(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (middle_is_valid(str[i]) == 1)
		{
			printf("invalid identifier\n");
			return (1);
		}
		i++;
	}
	return (0);
}

int	ft_unset(t_job **lst, char **env, int fd)
{
	t_job	*current;
	int		length;
	int		i;
	int		j;
	int     x;

	current = *lst;
	i = 0;
	x = 0;
	(void)fd;
	if (first_is_valid(current->arg[i][0]) == 1)
		printf("unset - invalid identifier\n");
		//write error message
	while (current->arg[i])
	{
		//if (first_is_valid(current->arg[i][0]) == 1)
			//printf("unset - not alphabet\n");
			//write error message
		if (check_middle_values(current->arg[i]) == 1)
		{
			printf("unset - invalid identifier\n");
			i++;
		}
		else
		{
			j = 0;
			length = get_length(current->arg[i]);
			while (env[j])
			{
				if (ft_strncmp(env[j], current->arg[i], length) == 0)
				{
					printf("unset - variable found and to be deleted\n");
					//delete line
					//delete_line(env, current->arg[i]);
					while (env[x])
					{
						if (ft_strncmp(env[x], current->arg[i], length) == 0)
							break ;
						x++;
					}
					env[x] = NULL;
					free(env[x]);
					return ;
					//break ;
				}
				j++;
			}
		}
		i++;
	}
}
*/

static int	unset_strncmp(const char *s1, const char *s2, size_t n)
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

void	ft_unset(t_job **lst, char **envp, int fd)
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
			if (current && unset_strncmp(current->str, temp->cmd, get_length(temp->cmd)) == 0)
			{
				next = current->next;
				printf("\tdeleting/freeing %s\n", current->str);
				free(current->str);
				free(current->next);
				free(current); //
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
				free(next->str);
				free(next->next);
				free(next);
				break ;
			}
			current = current->next;
		}
		i++;
	}
}
