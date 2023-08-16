#include "minishell.h"

int	middle_is_valid(char c)
{
	//0 = no problem
	//1 = problem
	if (c == '=' || c == '_') //c == '-' || c == '\\' || c == ' ' || ||  c == '|'
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
	if (c == '=')
		return (1);
	if (ft_isalnum(c) == 0 && c != '_')
		return (1);
	/*
	if (c >= '0' && c <= '9')
		return (1);
	else if (c == 92 || c == 95)
		return (0);
	else if (c>= 'a' && c <= 'z')
		return (0);
	else if (c >= 'A' || c <= 'Z')
		return (0);
	return (1);
	*/
	return (0);
}

int	check_middle(char *str)
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
	write(2, "bash: export: \'", 15);//
	write(2, str, get_length(str));//
	write(2, "\': not a valid identifier\n", 26);//
}
