#include "minishell.h"

void	ft_env(t_job **lst, char **env, int fd)
{
	int		i;
	t_job	*current;

	current = *lst;
	i = 0;
	if (current->arg)
	{
		write(fd, "env: \'", 6);
		write(fd, current->arg[0], get_length(current->arg[0]));
		write(fd, "\': No such file or directory\n", 29);
		return ;
	}
	while (env[i]) //
	{
		//length = get_length(env[i]);
		//write(fd, env[i], length);
		write(fd, env[i], get_length(env[i]));
		write(fd, "\n", 1);
		i++;
	}
}
