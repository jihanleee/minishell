#include "minishell.h"

void	cd_old_path(void)
{
	char	*new;

	new = find_param("OLDPWD");
	if (!new)
		cd_nonexist_error("\b\b");
	else if (access(new, F_OK) == -1)
		cd_nonexist_error(new);
	else if (is_dir(new) == FALSE)
		cd_dir_error(new);
	else if (access(new, X_OK) == 0)
	{
		change_update(new);
	}
	else
		cd_error(new);
	free(new);
}

void	cd_normal_path(t_job *current)
{
	char	*path;
	char	*path_modified;
	char	*new;

	path = getcwd(NULL, 0);
	path_modified = ft_strjoin(path, "/");
	new = ft_strjoin(path_modified, current->arg[0]);
	if (access(new, F_OK) == -1)
		cd_nonexist_error(current->arg[0]);
	else if (is_dir(new) == FALSE)
		cd_dir_error(current->arg[0]);
	else if (access(new, X_OK) == 0)
	{
		change_update(new);
	}
	else
		cd_error(current->arg[0]);
	free((free((free(path), path_modified)), new));
}

void	cd_to_home(void)
{
	char	*new;

	new = find_param("HOME");
	if (!new)
		write(2, "bash: HOME: No such file or directory\n", 38);
	else if (access(new, F_OK) == -1)
		cd_nonexist_error(new);
	else if (is_dir(new) == FALSE)
		cd_dir_error(new);
	else if (access(new, X_OK) == 0)
		change_update(new);
	else
		cd_error(new);
	free(new);
}

void	cd_absolute_path(t_job *current)
{
	char	*new;

	if (!current->arg[0][1])
		cd_to_home();
	else if (current->arg[0][1] == '/')
	{
		new = ft_strjoin(getenv("HOME"), &current->arg[0][1]);
		if (access(new, F_OK) == -1)
			cd_nonexist_error(new);
		else if (is_dir(new) == FALSE)
			cd_dir_error(new);
		else if (access(new, X_OK) == 0)
		{
			change_update(new);
		}
		else
			cd_error(new);
		free(new);
	}
}

void	cd_to_root(void)
{
	char	*cwd;
	char	*line;

	cwd = getcwd(NULL, 0);
	cd_unset("OLDPWD");
	line = ft_strjoin("OLDPWD=", cwd);
	add_to_env(line);
	free(cwd);
	free(line);
	chdir("/");
	cd_unset("PWD");
	add_to_env("PWD=/");
}
