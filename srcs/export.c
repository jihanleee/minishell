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
	/*
	if (c >= 'a' && c <= 'z')
		return (0);
	else if (c >= 'A' && c <= 'Z')
		return (0);
	return (1);
	*/
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
		printf("\targ number %d\n", i);
		printf("\tcurrent char is %c\n", current->arg[i][0]);
		if (flag == 0 && first_is_valid(current->arg[i][0]) == 1)
		{
			printf("export: invalid identifier\n");
			if (current->arg[i + 1])
				i++;
			else
				return (count);
		}
		j = 0;
		//if (flag == 0 && i != 0 && current->arg[i][j] != '=')
		//{
		//	printf("\ti has value %d\n", i);
		//	printf("\tone: new block break at %c\n", current->arg[i][j]);
		//	j++;
		//	count++;
		//}
		//else
		//{
			while (current->arg[i][j])
			{
				if (current->arg[i][j] == '\\')
					j++;
				if (current->arg[i][j] == '=')
					flag = 1;
				j++;
			}
		//}
		if (flag == 1)
		{
			flag = 0;
			printf("\t\ttwo: new block break at %c\n", current->arg[i][j - 1]);
			count++;
		}
		i++;
	}
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
		printf("\targ number %d\n", i);
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
			//printf("\ttwo: new block break at %c\n", current->arg[i][j - 1]);
			printf("\tcreating combine line #%d with length of %d\n", count + 1, length);
			//combine[count] = (char *)malloc(sizeof(char) * (length + 1));
			combine[count] = (char*) calloc(length + 1, sizeof(char));
			//if (!combine[count++])
			//if (!combine[count + 1])
			if (!combine[count])
				return (-1);
			length = 0;
		}
		if (flag == 0)
			length = 0;
		i++;
	}
	//combine[count] = (char *)malloc(1);
	//if (!combine[count])
	//	return (-1);
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
		//printf("\targ number %d\n", i);
		j = 0;
		//printf("before while loop one\n");
		while (current->arg[i] && first_is_valid(current->arg[i][j]) == 1)
			i++;
		//printf("before while loop two\n");
		while (current->arg[i][j] && combine[count])
		{
			//printf("\tchar: %c\n", current->arg[i][j]);
			if (current->arg[i][j] == '\\')
				j++;
			if (flag == 0 && current->arg[i][j] == '=' && j == 0)
				combine[count][index++] = ' ';
			//printf("after if one\n");
			//printf("\tcurrent char is %c\n", current->arg[i][j]);
			if (middle_is_valid(current->arg[i][j]) == 1)
			{
				printf("invalid identifier\n");
				return (1);	//error, invalid identifier
			}
			if (current->arg[i][j] == '=')
				flag = 1;
			//if (current->arg[i][j] == '\\')
			//	j++;
			if (middle_is_valid(current->arg[i][j]) == 0)
			{
				//printf("after if one\n");
				//printf("added %c\n", current->arg[i][j]);
				combine[count][index] = current->arg[i][j];
			}
			//printf("\tcurrent temp string is %s\n", save);
			index++;
			j++;
		}
		if (flag == 1)
		{
			flag = 0;
			//printf("\ttwo: new block break at %c\n", current->arg[i][j - 1]);
			//printf("\tcreating combine line #%d with length of %d\n", count + 1, length);
				//combine[count] = (char *)malloc(sizeof(char) * length + 1);
				//if (!combine[count++])
					//return (-1);
				//length = 0;
			//printf("before copy paste\n");
			//scopy_paste(&combine[count], &temp);
			//free(temp);
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
	//combine[count] = NULL;
	return (0);

}

static char	*get_command(char *line, int end)
{
	char	*result;
	int i;

	i = 0;
	//result = (char *)malloc(sizeof(char) * end + 1);
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
	//if (!combine)
	//	return ;
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
		env[x] = malloc(sizeof(char *) * (get_length(combine[i]) + 1));
		if (!env[x])
			return (-1);
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
	//int		flag; //0 means left side of =, 1 means right side of =
	//int		envp;
	int		count;
	int		i;
	int		x;

	printf("\tinside ft_export function\n");
	current = *lst;
	if (!current->arg)
		return (-1);
	else if (current->arg == NULL)
	{
		printf("invalid identifier\n");
		return (-1);	//error
	}
	if (current->arg[0] == NULL)
		//call env function
		ft_env(lst, env, fd);
	else
	{
		//loop through the whole arg 2d array to get count
		//count = get_block_count(lst) + 1;
		count = get_block_count(lst);
		printf("\t# of blocks = %d\n", count);
		if (count != 0)
		{
			//combine = (char **)malloc(sizeof(char *) * (count + 1));
			combine = (char **)ft_calloc(count + 1, sizeof(char *));
			//count = 0;
			if (!combine)
				return (-1);
			i = 0;
			malloc_combine_lines(combine, lst);
			printf("malloc combine lines done\n");
			if (fill_blocks(combine, lst) == 1)
			{
				printf("fill blocks gave error\n");
				while (count > 0)
					free(combine[count--]);
				free(combine);
				return (-1);	//error
			}
			printf("fill blocks done\n");
			printf("----------------------------------------\n");
			printf("reading blocks below...\n");
			read_blocks(combine, env);
			printf("before add blocks\n");
			add_blocks(combine, env);
			printf("after add blocks\n");
			while (count > 0)
					free(combine[count--]);
			free(combine);
			return (1);
			//printf("\n\n");
			//ft_env(lst, env, fd);
			//export_combined(combine);
				//read each line, check conditions, and add to env
			//free_combine(combine);
			//free(combine);
		}
	}
	//loop through combine's lines and free
	//free combine itself
}
