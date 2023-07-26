#include "minishell.h"

char	*find_param(char *p_name, char **envp)
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
