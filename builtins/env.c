#include "minishell.h"

void	ft_env(t_job **lst, char **env, int fd)
{
	int		i;
	int		length;
	t_job	*current;

	current = *lst;
	i = 0;
	if (current->arg)
	{
		printf("env - too many arguments error detected\n");
		return ;
	}
	while (env[i] && env[i][0])
	{
		length = get_length(env[i]);
		write(fd, env[i], length);
		write(fd, "\n", 1);
		i++;
	}
}
