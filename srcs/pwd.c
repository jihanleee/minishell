#include "minishell.h"

//static functions should be deleted later as 
//they already exist in another file + header file

/*static int	get_length(char *str)
{
	int	length;

	length = 0;
	while (str[length])
		length++;
	return (length);
}
*/

int	ft_pwd(t_pipe **lst, char **env, int fd)
{
	t_pipe	*current;
	char	*print;
	int		length;

	current = *lst;
	if (current->arg)	//pwd should not take any arguments after it
	{
		printf("pwd: too many arguments\n");
		//return int that indicates an error
		//error message = "pwd: too many arguments\n"
	}
	//print = find_param(current->cmd, env);
	print = getcwd(NULL, 0);
	//if (!print)
	//	return (1);
	length = get_length(print);
	ft_putstr_fd(print, fd);
	write(fd, "\n", 1);
	free(print);
	return (0);
}
