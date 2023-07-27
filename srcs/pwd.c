#include "minishell.h"

//2 static functions should be deleted later as 
//they already exist in another file + header file
static int	get_length(char *str)
{
	int	length;

	length = 0;
	while (str[length])
		length++;
	return (length);
}

static char	*find_param(char *p_name, char **envp)
{
	int		i;
	char	*str;

	str = 0;
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], p_name, ft_strlen(p_name)) == 0)
		{
			if (envp[i][ft_strlen(p_name)] == '=')
			{
				str = ft_strdup(ft_strchr(envp[i], '=') + 1);
				printf("%s\n", str);
				break ;
			}
		}
		i++;
	}
	return (str);
}


void	ft_pwd(t_pipe **lst, char **env)
{
	t_pipe	*current;
	char	*print;
	int		length;

	current = *lst;
	if (current->arg[0] != NULL)	//pwd should not take any arguments after it
	{
		//return int that indicates an error
		//error message = "pwd: too many arguments\n"
	}
	print = find_param(current->cmd, env);
	length = get_length(current->cmd);
	//write(next pipe's fd, &print, length);

}
