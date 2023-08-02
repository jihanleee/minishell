#include "minishell.h"

int	get_length(char *str)
{
	int	length;

	length = 0;
	while (str[length])
		length++;
	return (length);
}

void	ft_echo(t_pipe **lst, char **env, int fd)
{
	int		i;
	int		j;
	int		length;
	int		flag;
	char	*temp;
	t_pipe	*current;

	flag = 0;
	current = *lst;
	//printf("\tcurrently in echo function\n");
	//printf("\tArg 1: %s\n", current->arg[0]);
	//printf("\tArg 2: %s\n", current->arg[1]);
	//printf("before everything\n");
	i = 0;
	if (current->arg)
	{	j = 3;
		//printf("can't come here\n");
		while (current->arg[i] && ft_strncmp(current->arg[i], "-n", 2) == 0) //&& !current->arg[i + 2]
		{
			if (current->arg[i][2] == 0)
			{
				printf("\tdetected option -n\n");
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
		//printf("before while loop\n");
		printf("current string: %s\n", current->arg[i]);
		while (current->arg[i])
		{
			//printf("in while loop\n");
			//if (current->arg[i][0] == '\\')
			//{
				//printf("detected \\\n");
				//current->arg[i] = ++;
				//current->arg[i][1];
			//}
			//length = get_length(current->arg[i]);
			//temp = (char *)malloc(sizeof(char) * length);
			//if (!temp)
			//	return ;
			//printf("\t%s has length %d\n", current->arg[i], length);
			j = 0;
			while (current->arg[i][j])
			{
				//printf("current char is %c\n", current->arg[i][j]);
				if (current->arg[i][j] == '\\')
					j++;
				//printf("current char is %c\n", current->arg[i][j]);
				write(fd, &current->arg[i][j], 1);
				j++;
			}
			//write(fd, current->arg[i], length);
			i++;
			if (current->arg[i])
				write(fd, " ", 1);
		}
	}
	//printf("after while loop\n");
	if (flag == 0)
	{
		//printf("\tthere was no option so new line being printed\n");
		write(fd, "\n", 1);
	}
	//echo doesn't have any cases where an error should occur (i think)
	//so void function should be fine (i think)
}