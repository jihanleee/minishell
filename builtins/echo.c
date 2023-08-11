#include "minishell.h"

int	get_length(char *str)
{
	int	length;

	length = 0;
	while (str[length])
		length++;
	return (length);
}

void	ft_echo(t_job **lst, char **env, int fd)
{
	int		i;
	int		j;
	int		flag;
	t_job	*current;

	flag = 0;
	current = *lst;
	i = 0;
	if (current->arg)
	{	j = 3;
		while (current->arg[i] && ft_strncmp(current->arg[i], "-n", 2) == 0)
		{
			if (current->arg[i][2] == 0)
			{
				i++;
				flag = 1;
			}
			else if (current->arg[i][2] == 'n')
			{
				while(current->arg[i][j] == 'n')
					j++;
				if (current->arg[i][j] == 0)
				{
					i++;
					flag = 1;
				}
				else
					break;
			}
			else
				break;
		}
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
	if (flag == 0)
		write(fd, "\n", 1);
}
