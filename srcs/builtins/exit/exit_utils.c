#include "minishell.h"

bool	is_overflow(char *arg)
{
	int	i;

	i = 0;
	while (arg[i])
		i++;
	i--;
	if (arg[0] == '-')
	{
		if (ft_atol(arg) % 10 != (arg[i] - '0') * -1)
			return (1);
	}
	else
	{
		if (ft_atol(arg) % 10 != (arg[i] - '0'))
			return (1);
	}
	return (0);
}
