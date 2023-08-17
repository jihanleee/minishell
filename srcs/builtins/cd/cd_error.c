#include "minishell.h"

void	cd_nonexist_error(char *str)
{
	g_exit_stat = 1;
	write(2, "bash: cd: ", 10);
	write(2, str, get_length(str));
	write(2, ": No such file or directory\n", 28);
}

void	cd_dir_error(char *str)
{
	g_exit_stat = 1;
	write(2, "bash: cd: ", 10);
	write(2, str, get_length(str));
	write(2, ": Not a directory\n", 18);
}

void	cd_error(char *str)
{
	g_exit_stat = 1;
	write(2, "bash: cd: ", 10);
	write(2, str, get_length(str));
	write(2, ": No such file or directory\n", 28);
}

int	invalid_cd(char *path)
{
	int	i;

	i = 0;
	if (path[0] == '/' && path[1])
		return (1);
	while (path[i])
	{
		if (i != 0 && path[i] == '~')
			return (1);
		if (i != 0)
		{
			if (path[i - 1] == '/' && path[i] == '.' && path[i + 1] == '/')
				return (1);
		}
		i++;
	}
	return (0);
}
