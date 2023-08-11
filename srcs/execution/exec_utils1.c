#include "minishell.h"

void	free_arrays(char **str)
{
	int	i;

	i = 0;
	while (str[i])
		free(str[i++]);
	free(str);
}

void	error_exit(char *str, int exit_status)
{
	ft_putstr_fd(str, 2);
	exit(exit_status);
}

char	**bin_path(char **envp)
{
	int		i;
	int		j;
	char	**path;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			break ;
		i++;
	}
	j = 0;
	while (envp[i][j] && envp[i][j] != '=')
		j++;
	j++;
	path = ft_split(&(envp[i][j]), ':');
	if (path == 0)
		error_exit("malloc error\n", 1);
	return (path);
}

char	*file_path(char *cmd)
{
	if (access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	else
	{
		perror(cmd);
		exit(127);
	}
}

char	*find_cmd_path(char *cmd, char **envp)
{
	int		i;
	char	*slashed;
	char	*withcmd;
	char	**path;

	path = bin_path(envp);
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
		error_exit("command not found\n", 127);
	return (withcmd);
}
