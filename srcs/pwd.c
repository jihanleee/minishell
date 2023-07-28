#include "minishell.h"

//2 static functions should be deleted later as 
//they already exist in another file + header file
static int	get_length(char *str)
{
	int	length;

	length = 0;
	while (str[length])
		length++;
	return (length);
}

void	ft_pwd(t_pipe **lst, char **env)
{
	t_pipe	*current;
	char	*print;
	int		length;

	current = *lst;
	if (current->arg[0] != NULL)	//pwd should not take any arguments after it
	{
		//return int that indicates an error
		//error message = "pwd: too many arguments\n"
	}
	print = find_param(current->cmd, env);
	length = get_length(current->cmd);
	//write(next pipe's fd, &print, length);

}
