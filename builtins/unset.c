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
