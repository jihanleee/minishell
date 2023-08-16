#include "minishell.h"

int	get_length(char *str)
{
	int	length;

	length = 0;
	if (!str)
		return (0);
	while (str[length])
	{
		length++;
	}
	return (length);
}

int	print_args(t_job *current, int i, int fd)
{
	int	j;

	while (current->arg[i])
	{
		j = 0;
		while (current->arg[i][j])
		{
			if (current->arg[i][j] == '\\')
				j++;
			write(fd, &current->arg[i][j], 1);
			j++;
		}
		i++;
		if (current->arg[i])
			write(fd, " ", 1);
	}
}

void	ft_echo(t_job *current, int fd)
{
	int		i;
	int		j;
	int		flag;

	flag = 0;
	i = 0;
	if (current->arg)
	{	
		while (current->arg[i] && ft_strncmp(current->arg[i], "-n", 2) == 0)
		{
			j = 2;
			while (current->arg[i][j] == 'n')
				j++;
			if (current->arg[i][j] == 0)
			{
				i++;
				flag = 1;
			}
			else
				break ;
		}
		print_args(current, i, fd);
	}
	if (flag == 0)
		write(fd, "\n", 1);
}