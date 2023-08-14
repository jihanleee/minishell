#include "minishell.h"


static char	*change_directory(char **env, char *new, int fd)
{
	int i;

	i = 0;
	while(new[i] && new[i] != '/')
	{

	}
	if (new[i])
		return (&new[i + 1]);
	return(NULL);
}

static int invalid_cd(char *path)
{
	int i;

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
}


static void	cd_unset(char **env, char *arg)
{
	int		i;

	i = 0;
	while (env[i]) //어느 순간부터 에러 남 //expansion쪽 에러도 나서 같이 종합적으로 봐야할 것 같음
	{
		if (ft_strncmp(env[i], arg, get_length(arg)) == 0)
		{
			env[i] = NULL;
			free(env[i]);
			return ;
		}
		i++;
	}
}

static void	cd_export(char **env, char *arg, char *path)
{
	int length;
	int i;
	int j;
	int x;

	length = 0;	
	length += get_length(arg);
	length += get_length(path);
	i = 0;
	while(env[i])
		i++;
	env[i] = (char*)ft_calloc(length + 1, sizeof(char));
	if (!env[i])
		return ;
	j = -1;
	while (++j < get_length(arg))
		env[i][j] = arg[j];
	x = 0;
	while (x < get_length(path))
	{
		env[i][j] = path[x];
		j++;
		x++;
	}
}

static void	cd_old_path(t_job *current, char **env)
{
	char	*new;
	char	*cwd;

	new = find_param("OLDPWD", env);
	if (access(new, X_OK) == 0)
	{
		cd_unset(env, "OLDPWD=");
		cwd = getcwd(NULL, 0);
		cd_export(env, "OLDPWD=", cwd);
		free(cwd);
		chdir(new);
		cd_unset(env, "PWD=");
		cwd = getcwd(NULL, 0);
		cd_export(env, "OLDPWD=", cwd);
		free(cwd);
	}
	else
	{
		write(2, "bash: ", 6);
		write(2, new, get_length(new));
		write(2, ": No such file or directory\n", 28);
	}
	free(new);
}

static void cd_normal_path(t_job *current, char **env)
{
	char	*cwd;

	if (access(current->arg[0], X_OK) == 0)
	{
		cd_unset(env, "OLDPWD=");
		cwd = getcwd(NULL, 0);
		cd_export(env, "OLDPWD=", cwd);
		free(cwd);
		chdir(current->arg[0]);
		cd_unset(env, "PWD=");
		cwd = getcwd(NULL, 0);
		cd_export(env, "OLDPWD=", cwd);
		free(cwd);
	}
	else
	{
		write(2, "bash: ", 6);
		write(2, current->arg[0], get_length(current->arg[0]));
		write(2, ": No such file or directory\n", 28);
	}
}

static void	cd_to_home(t_job *current, char **env)
{
	char	*new;
	char	*cwd;

	new = getenv("HOME");
	if (access(new, X_OK) == 0)
	{
		cd_unset(env, "OLDPWD=");
		cwd = getcwd(NULL, 0);
		cd_export(env, "OLDPWD=", cwd);
		free(cwd);
		chdir(new);
		cd_unset(env, "PWD=");
		cwd = getcwd(NULL, 0);
		cd_export(env, "PWD=", cwd);
		free(cwd);
	}
	else
	{
		write(2, "bash: ", 6);
		write(2, new, get_length(new));
		write(2, ": No such file or directory\n", 28);
	}
	//free(new);
}

static void	cd_absolute_path(t_job *current, char **env)
{
	char	*new;
	char	*old;

	if (!current->arg[0][1])
		cd_to_home(current, env);
	else if (current->arg[0][1] == '/')
	{
		old = getcwd(NULL, 0);
		new = getenv("HOME");
		new = ft_strjoin(new, &current->arg[0][1]);
		if (access(new, X_OK) == 0)
		{
			cd_unset(env, "OLDPWD=");
			cd_export(env, "OLDPWD=", old);
			chdir(new);
			cd_unset(env, "PWD=");
			cd_export(env, "PWD=", new);
		}
		else
		{
			write(2, "bash: ", 6);
			write(2, new, get_length(new));
			write(2, ": No such file or directory\n", 28);
		}
		free(new);
		free(old);
	}
}

int	ft_cd(t_job **lst, char **env, int fd)
{
	t_job	*current;
	char	*path;
	char	*new;
	
	current = *lst;
	(void)fd;
	if (current->arg && current->arg[1])
		return (write(fd, "bash: cd: too many arguments\n", 29), 1);
	if (!current->arg || (current->arg[0][0] == '/' && !current->arg[0][1]))
		cd_to_home(current, env);	
	else if (current->arg[0][0] == '-' && !current->arg[0][1])
		cd_old_path(current, env);
	else if (current->arg[0][0] == '~' && (!current->arg[0][1] || current->arg[0][1] == '/'))
		cd_absolute_path(current, env);
	else
	{
		if (invalid_cd(current->arg[0]) == 1)
		{
			write(2, "bash: cd: ", 10);
			write(2, current->arg[0], get_length(current->arg[0]));
			write(2, ": No such file or directory\n", 28);
		}
		cd_normal_path(current, env);
	}
	return (0);
}
