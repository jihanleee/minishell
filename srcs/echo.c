#include <stdio.h>

int	get_length(char *str)
{
	int	length;

	length = 0;
	while (str[length])
		length++;
	return (length);
}

void	ft_echo(t_pipe **lst, char **env)
{
	int		i;
	int		length;
	int		flag;
	t_pipe	*current;

	i = 0;
	flag = 0;
	current = *lst;
	if (ft_strcmp(current->arg[i], "-n", 2) == 0)
		flag = 1;
	while (curent->arg[i])
	{
		length = get_length(current->arg[i]);
		//write(next pipe's fd, &arr[i], length);
		i++;
	}
	if (flag == 0)
		//write(next pipe's fd, "\n", 1);
	//echo doesn't have any cases where an error should occur (i think)
	//so void function should be fine (i think)
}