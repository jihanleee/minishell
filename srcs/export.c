#include "minishell.h"

/*
void	export_combined(char **combine)
{
	int	start;
	int	finish;
	int	flag;	//0 means left of = (or no = found)
	int	i;
	int	j;

	i = 0;
	while (combine[i])
	{
		j = 0;
		flag = 0;
		while (combine[i][j])
		{
			start = 0;
			finish = -1;
			//get start and finish index
			if (combine[i][j] == '=')
			{
				if (combine[i][j - 1] == ' ')
					printf("export: invalid identifier\n");
					//invalid identifier
					//break
				else
				{
					finish = j;
					flag = 1;
				}
			}
			if (finish != j - 1)
				finish = j - 1;
				
				//when find finish, break
			j++;
		}
		//if (finish != -1... needed?) && flag == 1
			//add_to_env(combine[i], start, index)
		//if flag == 0 (means no = sign found)
			//ignore line
		i++;
	}
}
*/

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

int	get_block_count(t_pipe **lst)
{
	t_pipe	*current;
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

int	malloc_combine_lines(char **combine, t_pipe **lst)
{
	t_pipe	*current;
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
			combine[count] = (char *)malloc(sizeof(char) * length + 1);
			if (!combine[count++])
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

int	fill_blocks(char **combine, t_pipe **lst)
{
	t_pipe	*current;
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
			printf("\tcurrent char is %c\n", current->arg[i][j]);
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
				printf("added %c\n", current->arg[i][j]);
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

/*
void	fill_blocks(char **combine, t_pipe **lst)
{
	t_pipe	*current;
	int		flag;
	int		i;
	int		j;
	int		a;
	int		b;

	a = 0;
	b = 0;
	flag = 0;
	i = 0;
	printf("\tinside fill_blocks\n");
	printf("\tbefore fill_blocks while loop\n");
	while (current->arg[i])
	{
		printf("\tinside fill_blocks while loop\n");
		j = 0;
		if (flag == 1 || (flag == 0 && current->arg[i][0] != '='))
		{
			combine[a++][b] = 0;
			b = 0;
		}
		else if (flag == 0 && current->arg[i][0] == '=')
			combine[a][b++] = ' ';
		while (current->arg[i][j])
		{
			if (current->arg[i][j] == '=')
				flag = 1;
			combine[a][b] = current->arg[i][j];
			j++;
		}
		i++;
	}
}
*/

static char	*get_command(char *line, int end)
{
	char	*result;
	int i;

	i = 0;
	result = (char *)malloc(sizeof(char) * end + 1);
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
		printf("block #%d:\n\t");
		while (combine[i][j])
		{
			printf("%c", combine[i][j]);
			if (combine[i][j] == '=')
				end = j;
			j++;
		}
		printf("\n");
		arg = get_command(combine[i], end);
		printf("looking for var %s in env\n", arg);
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
		env[x] = malloc(sizeof(char *) * (get_length(combine[i] + 1)));
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

int	ft_export(t_pipe **lst, char **env, int fd)
{
	t_pipe	*current;
	char	**combine;
	//int		flag; //0 means left side of =, 1 means right side of =
	//int		envp;
	int		count;
	int		i;
	int		x;

	printf("\tinside ft_export function\n");
	current = *lst;
	if (!current->arg)
		return (0);
	else if (current->arg == NULL)
	{
		printf("invalid identifier\n");
		return (1);	//error
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
			combine = malloc(sizeof(char *) * (count + 1));
			count = 0;
			if (!combine)
				return (-1);
			i = 0;
			malloc_combine_lines(combine, lst);
			printf("malloc combine lines done\n");
			if (fill_blocks(combine, lst) == 1)
			{
				while (count > 0)
					free(combine[count--]);
				free(combine);
				return (1);	//error
			}
			printf("fill blocks done\n");
			printf("reading blocks below...\n");
			printf("----------------------------------------\n");
			read_blocks(combine, env);
			printf("before add blocks\n");
			add_blocks(combine, env);
			printf("after add blocks\n");
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
