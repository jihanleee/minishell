#include "minishell.h"

int	first_is_valid(char c)
{
	if (c == '=')
		return (1);
	if (ft_isalpha(c) == 0 && c != '_')
		return (1);
	return (0);
}

int	middle_is_valid(char *str)
{
	int	i;

	i = 0;
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

int	middle_error_case(char *str)
{
	int	i;
	int	flag;

	i = 0;
	flag = 0;
	while (str[i])
	{
		if (str[i] == '\\')
			i++;
		if (str[i] == '=' && i == 0)
			return (1);
		if (str[i] == '=' && i != 0)
		{	
			flag = 1;
			break ;
		}
		if (ft_isalnum(str[i]) == 0 && str[i] != '_')
			return (1);
		i++;
	}
	if (flag == 0)
		return (1);
	return (0);
}

void	export_error(char *str)
{
	write(2, "bash: export: \'", 15);
	write(2, str, get_length(str));
	write(2, "\': not a valid identifier\n", 26);
	g_exit_stat = 1;
}
