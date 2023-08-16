#include "minishell.h"

int	get_block_count(t_job **lst)
{
	t_job	*current;
	int	count;
	int	i;
	int	j;

	current = *lst;
	count = 0;
	i = 0;
	//printf("in get block count\n");
	while (current->arg[i])
	{
		if (first_is_valid(current->arg[i][0]) == 1)
		{
			//printf("get block error\n");
			export_error(current->arg[i]);
			if (current->arg[i + 1])
				i++;
			else
				return (count);
		}
		j = 0;
		if (check_middle(current->arg[i]) == 0) //valid arg
			count++;
		i++;
	}
	//ft_printf("block count: %d\n", count);
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
	i = -1;
	while (current->arg[++i])
	{
		if (first_is_valid(current->arg[i][0]) == 0 && check_middle(current->arg[i]) == 0)
		{
			length = get_length(current->arg[i]);
			combine[count] = (char*)ft_calloc(length + 1, sizeof(char));
			count++;
		}
	}
	return (0);
}

int	middle_error_case(char *str)
{
	int	i;

	i = 0;
	//printf("\tlooking at %s\n", str);
	while (str[i])
	{
		if (str[i] == '\\')
			i++;
		if (str[i] == '=' && i == 0)
			return (1);
		if (str[i] == '=' && i != 0)
			break ;
		if (ft_isalnum(str[i]) == 0 && str[i] != '_')
			return (1);
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
	//printf("current line: %s\n", current->arg[0]);
	while (current->arg[i]) // && combine[count]
	{
		j = 0;
		//printf("current line: %s\n", current->arg[i]);
		//if (first_is_valid(current->arg[i][j]) == 1)
		//	export_error(current->arg[i]);
		if (check_middle(current->arg[i]) == 0)
		{
			//printf("adding line to combine: \n\t%s\n", current->arg[i]);
			combine[count] = ft_strdup(current->arg[i]);
			count++;
		}
		else if (middle_error_case(current->arg[i]) == 1)
			export_error(current->arg[i]);
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

static void	export_unset(char *key)
{
	t_env	**env;
	t_env	*current;
	t_env	*next;
	t_job	*temp;

	env = get_env_address();
	current = (*env);
	if (current && current->str)
	{
		if (current && unset_strncmp(current->str, key, get_length(key)) == 0)
		{
			next = current->next;
			current = next;
		}
	}
	while (current)
	{
		next = current->next;
		if (next && unset_strncmp(next->str, key, get_length(key)) == 0)
		{
			current->next = current->next->next;
			break ;
		}
		current = next;
	}
}



void	read_blocks(char **combine)
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
		while (combine[i][j])
		{
			if (combine[i][j] == '=')
			{
				end = j;
			}
			j++;
		}
		arg = get_command(combine[i], end);
		export_unset(arg);
		free(arg);
		i++;
	}
}

int	add_blocks(char **combine)
{
	t_env	**env;
	t_env	*current;
	t_env	*new;
	int		i;

	env = get_env_address();
	i = 0;
	if (*env == NULL)
	{
		*env = (t_env *)ft_calloc(1, sizeof(t_env));
		(*env)->str = ft_strdup(combine[i++]);
	}
	current = (*env);
	while (current && current->next != NULL)
		current = current->next;
	while (combine[i])
	{
		current->next = (t_env *)ft_calloc(1, sizeof (t_env));
		current->next->str = ft_strdup(combine[i]);
		current = current->next;
		i++;
	}
	return (0);
}

int	ft_export(t_job **lst, int fd)
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
		//ft_putstr_fd("invalid identifier\n", 2);
		export_error("");
		return (-1);	//error
	}
	if (current->arg[0] == NULL) //export만 들어오면
		ft_env(lst, fd);
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
				free_arrays(combine);
				return (-1);
			}
			read_blocks(combine);
			add_blocks(combine);
			free_arrays(combine);
			return (1);
		}
	}
}
