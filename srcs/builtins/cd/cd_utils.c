#include "minishell.h"

int	invalid_cd(char *path)
{
	int	i;

	i = 0;
	if (path[0] == '/' && path[1])
		return (1);
	while (path[i])
	{
		if (i != 0 && path[i] == '~')
			return (1);
		if (i != 0)
		{
			if (path[i - 1] == '/' && path[i] == '.' && path[i + 1] == '/')
				return (1);
		}
		i++;
	}
}

int	cd_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while ((s1[i] || s2[i]) && i < n)
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}

void	cd_unset(char *arg)
{
	t_env	**env;
	t_env	*current;
	t_env	*next;

	env = get_env_address();
	current = (*env);
	if (((*env) && (*env)->str) && \
		(cd_strncmp(current->str, arg, get_length(arg)) == 0))
	{
		next = current->next;
		free((free(current->str), current));
		(*env) = next;
	}
	current = (*env);
	while (current)
	{
		next = current->next;
		if (next && cd_strncmp(next->str, arg, get_length(arg)) == 0)
		{
			current->next = current->next->next;
			free((free(next->str), next));
			break ;
		}
		current = current->next;
	}
}

void	cd_export(char *arg, char *path)
{
	t_env	**env;
	t_env	*current;

	env = get_env_address();
	current = (*env);
	cd_unset(arg);
	while (current && current->next != NULL)
		current = current->next;
	current->next = (t_env *)ft_calloc(1, sizeof(t_env));
	current->next->str = ft_strjoin(arg, path);
	current->next->next = NULL;
}

bool	is_dir(char *path)
{
	DIR	*dir;

	dir = opendir(path);
	if(dir)
	{
		closedir(dir);
		return (TRUE);
	}
	else 
		return (FALSE);
}
