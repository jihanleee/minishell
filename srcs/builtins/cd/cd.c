#include "minishell.h"

static void	cd_old_path(void)
{
	char	*new;
	char	*cwd;

	new = find_param("OLDPWD");
	if (is_dir(new) == FALSE)
		cd_dir_error(new);
	else if (access(new, X_OK) == 0)
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
		cd_error(new);
	}
	free(new);
}

static void	cd_normal_path(t_job *current)
{
	char	*cwd;

	if (is_dir(current->arg[0]) == FALSE)
		cd_dir_error(current->arg[0]);
	else if (access(current->arg[0], X_OK) == 0)
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
		cd_error(current->arg[0]);
	}
}

static void	cd_to_home(void)
{
	char	*new;
	char	*cwd;

	new = getenv("HOME");
	if (!new)
		write(2, "bash: HOME: No such file or directory\n", 38);
	else if (is_dir(new) == FALSE)
		cd_dir_error(new);
	else if (access(new, X_OK) == 0)
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
		cd_error(new);
	}
}

static void	cd_absolute_path(t_job *current)
{
	char	*new;
	char	*old;

	if (!current->arg[0][1])
		cd_to_home();
	else if (current->arg[0][1] == '/')
	{
		old = getcwd(NULL, 0);
		new = ft_strjoin(getenv("HOME"), &current->arg[0][1]);
		if (is_dir(new) == FALSE)
			cd_dir_error(new);
		else if (access(new, X_OK) == 0)
		{
			cd_export("OLDPWD=", old);
			chdir(new);
			cd_export("PWD=", new);
		}
		else
		{
			cd_error(new);
		}
		free((free(new), old));
	}
}

int	ft_cd(t_job **lst, int fd)
{
	t_job	*current;
	char	*cwd;

	current = *lst;
	(void)fd;
	if (current->arg && current->arg[1])
		return (write(2, "bash: cd: too many arguments\n", 29), 1);
	cwd = getcwd(NULL, 0);
	if (path_compare(current->arg[0], cwd) == 0)
		return (0);
	free(cwd);
	if (!current->arg || (current->arg[0][0] == '/' && !current->arg[0][1]))
		cd_to_home();
	else if (current->arg[0][0] == '-' && !current->arg[0][1])
		cd_old_path();
	else if (current->arg[0][0] == '~' && \
		(!current->arg[0][1] || current->arg[0][1] == '/'))
		cd_absolute_path(current);
	else
	{
		if (invalid_cd(current->arg[0]) == 1)
			cd_error(current->arg[0]);
		cd_normal_path(current);
	}
	return (0);
}
