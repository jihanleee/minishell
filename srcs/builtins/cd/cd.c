#include "minishell.h"

static void	cd_old_path(t_job *current)
{
	char	*new;
	char	*cwd;

	new = find_param("OLDPWD");
	if (access(new, X_OK) == 0)
	{
		cwd = getcwd(NULL, 0);
		cd_export("OLDPWD=", cwd);
		free(cwd);
		chdir(new);
		cwd = getcwd(NULL, 0);
		cd_export("PWD=", cwd);
		free(cwd);
	}
	else
	{
		write(2, "bash: cd: ", 10);
		write(2, new, get_length(new));
		write(2, ": No such file or directory\n", 28);
	}
	free(new);
}

static void	cd_normal_path(t_job *current)
{
	char	*cwd;

	if (access(current->arg[0], X_OK) == 0)
	{
		cwd = getcwd(NULL, 0);
		cd_export("OLDPWD=", cwd);
		free(cwd);
		chdir(current->arg[0]);
		cwd = getcwd(NULL, 0);
		cd_export("PWD=", cwd);
		free(cwd);
	}
	else
	{
		write(2, "bash: cd: ", 10);
		write(2, current->arg[0], get_length(current->arg[0]));
		write(2, ": No such file or directory\n", 28);
	}
}

static void	cd_to_home(t_job *current)
{
	char	*new;
	char	*cwd;

	new = getenv("HOME");
	if (!new)
		write(2, "bash: HOME: No such file or directory\n", 38);
	else if (access(new, X_OK) == 0)
	{
		cwd = getcwd(NULL, 0);
		ft_printf("\tcwd = %s\n", cwd);
		cd_export("OLDPWD=", cwd);
		free(cwd);
		chdir(new);
		cwd = getcwd(NULL, 0);
		cd_export("PWD=", cwd);
		free(cwd);
	}
	else
	{
		write(2, "bash: cd: ", 10);
		write(2, new, get_length(new));
		write(2, ": No such file or directory\n", 28);
	}
}

static void	cd_absolute_path(t_job *current)
{
	char	*new;
	char	*old;

	if (!current->arg[0][1])
		cd_to_home(current);
	else if (current->arg[0][1] == '/')
	{
		old = getcwd(NULL, 0);
		new = getenv("HOME");
		new = ft_strjoin(new, &current->arg[0][1]);
		if (access(new, X_OK) == 0)
		{
			cd_export("OLDPWD=", old);
			chdir(new);
			cd_export("PWD=", new);
		}
		else
		{
			write(2, "bash: cd: ", 10);
			write(2, new, get_length(new));
			write(2, ": No such file or directory\n", 28);
		}
		free(new);
		free(old);
	}
}

int	ft_cd(t_job **lst, int fd)
{
	t_job	*current;
	char	*path;
	char	*new;

	current = *lst;
	if (current->arg && current->arg[1])
		return (write(fd, "bash: cd: too many arguments\n", 29), 1);
	if (!current->arg || (current->arg[0][0] == '/' && !current->arg[0][1]))
		cd_to_home(current);
	else if (current->arg[0][0] == '-' && !current->arg[0][1])
		cd_old_path(current);
	else if (current->arg[0][0] == '~' && \
		(!current->arg[0][1] || current->arg[0][1] == '/'))
		cd_absolute_path(current);
	else
	{
		if (invalid_cd(current->arg[0]) == 1)
		{
			write(2, "bash: cd: ", 10);
			write(2, current->arg[0], get_length(current->arg[0]));
			write(2, ": No such file or directory\n", 28);
		}
		cd_normal_path(current);
	}
	return (0);
}
