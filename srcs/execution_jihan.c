#include "minishell.h"

void	free_arrays(char **str)
{
	int	i;

	i = 0;
	while (str[i])
		free(str[i++]);
	free(str);
}

void	error_exit(char *str)
{
	ft_putstr_fd(str, 2);
	exit(EXIT_FAILURE);
}

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
		error_exit("malloc error\n");
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
	if (!withcmd)
		error_exit("command not found\n");
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
	if (!result)
		return (0);
	result[0] = jobs->cmd;
	i = 1;
	while (i < size)
	{
		result[i] = jobs->arg[i - 1];
		i++;
	}
	return (result);
}

int	find_inout_fd(t_job *job, int *infd, int *outfd)
{
	if (job->in)
		*infd = open(job->infile, O_RDONLY);
	else if (job->prev && job->in == 0)
		*infd = job->prev->pipefd[0];
	else
		*infd = 0;
	if (job->out == 3)
		*outfd = open(job->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (job->out == 4)
		*outfd = open(job->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (job->next && job->out == 0)
		*outfd = job->pipefd[1];
	else if (!job->next)
		*outfd = 1;
	if (*outfd == -1 || *infd == -1)
		return (-1);
	return (0);
}

void	non_builtin_child(t_job *job, char **envp)
{
	pid_t	cpid;
	int		infd;
	int		outfd;
	char	*cmd_path;

	pipe(job->pipefd);
	cpid = fork();
	if (cpid == 0)
	{
		if (job->cmd == 0)
			exit(1);
		close(job->pipefd[0]);
		if (find_inout_fd(job, &infd, &outfd) == -1)
			error_exit("fd error\n");
		dup2(outfd, 1);
		dup2(infd, 0);
		cmd_path = find_cmd_path(job->cmd, envp);
		execve(cmd_path, get_argv(job), envp);
	}
	close(job->pipefd[1]);
	if (job->prev)
		close(job->prev->pipefd[0]);
}

void	builtin_child(t_job *job, char **envp)
{
	pid_t	cpid;
	int		infd;
	int		outfd;
	char	*cmd_path;

	pipe(job->pipefd);
	cpid = fork();
	if (cpid == 0)
	{
		close(job->pipefd[0]);
		if (job->cmd == 0)
			exit(1);
		if (job->in == -1)
			exit(1);
		if (find_inout_fd(job, &infd, &outfd) == -1)
			error_exit("fd error\n");
		dup2(infd, 0);
		exec_builtin(job, envp, outfd);
		exit(0);
	}
	close(job->pipefd[1]);
	if (job->prev)
		close(job->prev->pipefd[0]);
}

void	execute_jobs(t_job *jobs, char **envp)
{
	t_job	*current;
	int		stat;
	int		i;

	if (!jobs)
		return ;
	i = 0;
	current = jobs;
	while (current)
	{
		i++;
		if (current->cmd && check_builtin(current->cmd))
			builtin_child(current, envp);
		else if (current->cmd)
			non_builtin_child(current, envp);
		current = current->next;
	}
	while (i-- > 0)
		wait(&stat);
	ft_printf("stat :%d\n", stat);
}
