#include "minishell.h"

int	ft_pwd(t_job **lst, int fd)
{
	t_job	*current;
	char	*print;
	int		length;

	current = *lst;
	/*
	if (current->arg)	//pwd should not take any arguments after it
	{
		//pwd뒤에 뭐가 들어오면 안된다고 생각했는데 보니까 pwd는 뒤에 뭐가 와도 그냥 작동하는듯..?
		printf("pwd: too many arguments\n");
	}
	*/
	print = getcwd(NULL, 0);
	length = get_length(print);
	ft_putstr_fd(print, fd);
	write(fd, "\n", 1);
	free(print);
	return (0);
}
