#include "minishell.h"

char	**bin_path(char **envp)
{
	int		i;
	int		j;
	char	**path;
	char	*tmp;

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
	{
		ft_putstr_fd("malloc error\n", 2);
		exit(EXIT_FAILURE);
	}
	return (path);
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
		if (withcmd == 0)
			return (NULL);
		if (access(withcmd, X_OK) == 0)
			break ;
		i++;
	}
	if (path[i] == 0)
	{
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": command not found\n", 2);
		exit(EXIT_FAILURE);
	}
	return (withcmd);
}

char	**get_argv(t_job *jobs)
{
	char	**result;
	int		i;
	int		size;

	if (!jobs->cmd)
		return (0);
	i = 0;
	if (jobs->arg)
		while (jobs->arg[i])
			i++;
	size = i + 1;
	result = (char **)calloc(size + 1, sizeof(char *));
	result[0] = jobs->cmd;
	i = 1;
	while (i < size)
	{
		result[i] = jobs->arg[i - 1];
		i++;
	}
	return (result);
}

void	exec_child(t_job *job, char **envp)
{
	pid_t	cpid;
	int		infd;
	char	*cmd_path;
	char	buf;

	pipe(job->pipefd);
	cpid = fork();
	if (cpid == 0)
	{
		cmd_path = find_cmd_path(job->argv[0], envp);
		close(job->pipefd[0]);
		if (job->next && job->out == 0)
			dup2(job->pipefd[1], 1);
		if (job->prev && job->in == 0)
			dup2(job->prev->pipefd[0], 0);
		execve(cmd_path, job->argv, envp);
	}
	close(job->pipefd[1]);
	if (job->prev)
		close(job->prev->pipefd[0]);
}

void	execute_jobs(t_job *jobs, char **envp)
{
	t_job	*current;
	int		i;

	i = 0;
	current = jobs;
	while (current)
	{
		i++;
		current->argv = get_argv(current);
		exec_child(current, envp);
		current = current->next;
	}
	while (i-- > 0)
		wait(0);
}