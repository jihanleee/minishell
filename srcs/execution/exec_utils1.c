#include "minishell.h"

void	free_arrays(char **str)
{
	int	i;

	i = 0;
	while (str[i])
		free(str[i++]);
	free(str);
}

void	error_exit(char *str, int exit_status, t_job *job)
{
	rl_clear_history();
	clear_env();
	clear_jobs(job);
	ft_putstr_fd(str, 2);
	exit(exit_status);
}

char	**bin_path(t_job *job)
{
	t_env	*env;
	int		i;
	char	**path;

	env = *get_env_address();
	while (env)
	{
		if (ft_strncmp(env->str, "PATH=", 5) == 0)
			break ;
		env = env->next;
	}
	if (!env)
	{
		path = ft_split("/bin", ' ');
		return (path);
	}
	i = 0;
	while (env->str[i] && env->str[i] != '=')
		i++;
	i++;
	path = ft_split(&(env->str[i]), ':');
	if (path == 0)
		error_exit("malloc error\n", 1, job);
	return (path);
}

char	*file_path(char *cmd, t_job *job)
{
	if (access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	else
	{
		perror(cmd);
		rl_clear_history();
		clear_env();
		clear_jobs(job);
		exit(127);
	}
}

char	*find_cmd_path(char *cmd, t_job *job)
{
	int		i;
	char	*slashed;
	char	*withcmd;
	char	**path;

	path = bin_path(job);
	i = 0;
	while (path[i])
	{
		slashed = ft_strjoin(path[i], "/");
		withcmd = ft_strjoin(slashed, cmd);
		if (slashed)
			free(slashed);
		if (withcmd == 0)
			return (NULL);
		if (access(withcmd, X_OK) == 0)
			break ;
		free(withcmd);
		withcmd = 0;
		i++;
	}
	free_arrays(path);
	if (!withcmd || cmd[0] == '\0')
		error_exit("command not found\n", 127, job);
	return (withcmd);
}
