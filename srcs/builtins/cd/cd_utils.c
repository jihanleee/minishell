#include "minishell.h"

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

int	path_compare(char *s1, char *s2)
{
	int	lengthone;
	int	lengthtwo;
	int	i;

	lengthone = get_length(s1);
	lengthtwo = get_length(s2);
	if (lengthone != lengthtwo)
		return (1);
	i = 0;
	while (s1[i] && s2[i])
	{
		if (s1[i] != s2[i])
			return (1);
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

bool	is_dir(char *path)
{
	DIR	*dir;

	dir = opendir(path);
	if (dir)
	{
		closedir(dir);
		return (TRUE);
	}
	return (FALSE);
}
