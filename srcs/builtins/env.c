#include "minishell.h"

void	ft_env(t_job **lst, int fd)
{
	int		i;
	t_job	*job;
	t_env	**env;
	t_env	*current;

	//printf("inside export-add blocks function\n");
	job = *lst;
	i = 0;
	if (job->arg)
	{
		write(fd, "env: \'", 6);
		write(fd, job->arg[0], get_length(job->arg[0]));
		write(fd, "\': No such file or directory\n", 29);
		return ;
	}
	//
	env = get_env_address();
	current = (*env);
	while (current)
	{
		write(fd, current->str, get_length(current->str));
		write(fd, "\n", 1);
		current = current->next;
	}
	/*
	while (env[i]) //
	{
		//length = get_length(env[i]);
		//write(fd, env[i], length);
		write(fd, env[i], get_length(env[i]));
		write(fd, "\n", 1);
		i++;
	}
	*/
}
