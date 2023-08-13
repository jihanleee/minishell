#include "minishell.h"

int	middle_is_valid(char c)
{
	//0 = no problem
	//1 = problem
	if (c == '=' || c == '_' || c == '-' || c == '\\' || c == ' ' || c == '|')
		return (0);
	else if (c == '(' || c == ')')
		return (0);
	else if (c >= 'a' && c <= 'z')
		return (0);
	else if (c >= 'A' && c <= 'Z')
		return (0);
	else if (c >= '0' && c <= '9')
		return (0);
	return (1);
}

int	first_is_valid(char c)
{
	//0 = no problem
	//1 = probem
	if (c >= '0' && c <= '9')
		return (1);
	else if (c == 92 || c == 95)
		return (0);
	else if (c>= 'a' && c <= 'z')
		return (0);
	else if (c >= 'A' || c <= 'Z')
		return (0);
	return (1);
}

int	get_block_count(t_job **lst)
{
	t_job	*current;
	int	count;
	int	flag;
	int	i;
	int	j;

	current = *lst;
	flag = 0;
	count = 0;
	i = 0;
	while (current->arg[i])
	{
		if (flag == 0 && first_is_valid(current->arg[i][0]) == 1)
		{
			write(2, "bash: export: \'", 15);
			write(2, current->arg[i], get_length(current->arg[i]));
			write(2, "\': not a valid identifier\n", 26);
			if (current->arg[i + 1])
				i++;
			else
				return (count);
		}
		j = 0;
		while (current->arg[i][j])
		{
			if (current->arg[i][j] == '\\')
				j++;
			if (current->arg[i][j] == '=')
				flag = 1;
			j++;
		}
		if (flag == 1)
		{
			flag = 0;
			count++;
		}
		i++;
	}
	printf("number of blocks: %d\n", count);
	return (count);
}

int	malloc_combine_lines(char **combine, t_job **lst)
{
	t_job	*current;
	int	length;
	int count;
	int	flag;
	int	i;
	int	j;

	current = *lst;
	flag = 0;
	count = 0;
	length = 0;
	i = 0;
	while (current->arg[i])
	{
		j = 0;
		while (current->arg[i][j])
		{
			if (current->arg[i][j] == '=')
				flag = 1;
			length++;
			j++;
		}
		if (flag == 1)
		{
			flag = 0;
			combine[count] = (char*)ft_calloc(length + 1, sizeof(char));
			//if (!combine[count])
			//	return (-1);
			count++;
			length = 0;
		}
		if (flag == 0)
			length = 0;
		i++;
	}
	return (0);

}

int	fill_blocks(char **combine, t_job **lst)
{
	t_job	*current;
	int	index;
	int count;
	int	flag;
	int	i;
	int	j;

	current = *lst;
	flag = 0;
	count = 0;
	index = 0;
	i = 0;
	while (current->arg[i] && combine[count])
	{
		j = 0;
		while (current->arg[i] && first_is_valid(current->arg[i][j]) == 1)
			i++;
		while (current->arg[i][j] && combine[count])
		{
			if (current->arg[i][j] == '\\')
				j++;
			if (flag == 0 && current->arg[i][j] == '=' && j == 0)
				combine[count][index++] = ' ';
			if (middle_is_valid(current->arg[i][j]) == 1)
			{
				write(2, "bash: export: \'", 15);//
				write(2, current->arg[i], get_length(current->arg[i]));//
				write(2, "\': not a valid identifier\n", 26);//
				return (1);	//error, invalid identifier
			}
			if (current->arg[i][j] == '=')
				flag = 1;
			if (middle_is_valid(current->arg[i][j]) == 0)
				combine[count][index] = current->arg[i][j];
			index++;
			j++;
		}
		if (flag == 1)
		{
			flag = 0;
			count++;
			index = 0;
		}
		else if (flag == 0)
		{
			if (current->arg[i + 1][0] != '=')
			{
				while (index >= 0)
				{
					combine[count][index] = 0;
					index--;
				}
				index = 0;
			}
		}
		i++;
	}
	i = 0;
	while (combine[i])
	{
		printf("combine: index %d has %s\n", i, combine[i]);
		i++;
	}
	return (0);
}

static char	*get_command(char *line, int end)
{
	char	*result;
	int i;

	i = 0;
	result = (char*)ft_calloc(end + 1, sizeof(char));
	if (!result)
		return (NULL);
	while(i < end)
	{
		result[i] = line[i];
		i++;
	}
	return (result);
}


static void	export_unset(char **env, char *arg)
{
	int		length;
	int		i;

	i = 0;
	if (arg)
	{
		length = get_length(arg);
		while (env[i])
		{
			if (ft_strncmp(env[i], arg, length) == 0)
			{
				env[i] = NULL;
				free(env[i]);
				return ;
			}
			i++;
		}
	}
}

void	read_blocks(char **combine, char **env)
{
	int i;
	int j;
	int 	end;
	char	*arg;

	i = 0;
	end = 0;
	while (combine[i])
	{
		j = 0;
		printf("block #%d:\n", i + 1);
		while (combine[i][j])
		{
			printf("\tcombine has char %c\n", combine[i][j]);
			if (combine[i][j] == '=')
			{
				printf("\t\tinside if statement\n");
				end = j;
			}
			j++;
		}
		printf("\n");
		printf("before get_command is called\n");
		arg = get_command(combine[i], end);
		//printf("looking for var %s in env\n", arg);
		export_unset(env, arg);
		free(arg);
		i++;
	}
}


int	add_blocks(char **combine, char **env)
{
	int i;
	//int j;
	int x;
	//char *current;

	i = 0;
	x = 0;
	//current = **env;
	printf("\tcurrently in add blocks\n");
	//while (current[x])
	//	x++;
	//current[x] = malloc(sizeof(char *) * get_length(combine[i] + 1));
	//if (!current[x])
	//		return (-1);
	while (env[x])
		x++;
	//while (combine[i] && combine[i] != NULL)
	while (combine[i])
	{
		printf("\tcombine[i] is %s\n", combine[i]);
		//env[x] = malloc(sizeof(char *) * (get_length(combine[i]) + 1));
		//if (!env[x])
		//	return (-1);
		env[x] = combine[i];
		printf("\tenv now has %s\n", env[x]);
		printf("\tcombine had %s\n", combine[i]);
		x++;
		i++;
	}
	env[x] = NULL;
	return (0);
}

/*
void	free_combine(char **combine)
{
	int i;

	i = 0;
	while (combine[i])
		i++;
	while (i >= 0)
		free(combine[i--]);
	//free(combine);
}
*/

int	ft_export(t_job **lst, char **env, int fd)
{
	t_job	*current;
	char	**combine;
	int		count;
	int		i;
	int		x;

	current = *lst;
	if (!current->arg)
		return (-1);
	else if (current->arg == NULL) //?
	{
		printf("invalid identifier\n");
		return (-1);	//error
	}
	if (current->arg[0] == NULL) //export만 들어오면
		ft_env(lst, env, fd);
	else
	{
		count = get_block_count(lst);
		if (count != 0)
		{
			combine = (char **)ft_calloc(count + 1, sizeof(char *));
			if (!combine)
				return (-1);
			i = 0;
			malloc_combine_lines(combine, lst);
			if (fill_blocks(combine, lst) == 1) //fill blocks gave error
			{
				while (count > 0)
					free(combine[count--]);
				free(combine);
				return (-1);
			}
			read_blocks(combine, env);
			add_blocks(combine, env);
			//while (count > 0)
			//		free(combine[count--]);
			free(combine);
			return (1);
		}
	}
}
