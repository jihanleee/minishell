#include "minishell.h"

void	ft_env(t_job **lst, char **env, int fd)
{
	int		i;
	int		length;
	t_job	*current;

	current = *lst;
	printf("inside env function\n\n");
	i = 0;
	//if (current->arg[0] != NULL)
	if (current->arg)
	{
		printf("env - too many arguments error detected\n");
		//printf("1st line of env has \n\t%s\n", env[i]);
		return ;
		//error
		//error message: 
			//env: ‘(first arg)’: No such file or directory
	}
	//printf("before this line\n");
	//printf("1st line of env has \n\t%s\n", env[i]);
	//printf("after this line\n");
	//printf("%dth line of env has %s\n", i + 1, env[i]);
	while (env[i] && env[i][0])
	{
		//printf("\tno seg fault for %d\n", i);
		length = get_length(env[i]);
		write(fd, env[i], length);
		write(fd, "\n", 1);
		i++;
	}
	//printf("env has index %d\n", i);
}
