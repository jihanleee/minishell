#include "minishell.h"

int	ft_pwd(t_job **lst, int fd)
{
	t_job	*current;
	char	*print;
	int		length;

	current = *lst;
	print = getcwd(NULL, 0);
	length = get_length(print);
	ft_putstr_fd(print, fd);
	write(fd, "\n", 1);
	free(print);
	return (0);
}
