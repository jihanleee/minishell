#include "minishell.h"

int	ft_pwd(int fd)
{
	char	*print;

	print = getcwd(NULL, 0);
	ft_putstr_fd(print, fd);
	write(fd, "\n", 1);
	free(print);
	return (0);
}
