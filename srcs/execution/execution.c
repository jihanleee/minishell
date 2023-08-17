#include "minishell.h"

void	non_builtin_child(t_job *job)
{
	pid_t	cpid;
	char	*cmd_path;
	char	**argv;
	char	**envp;

	cpid = fork();
	if (cpid == 0)
	{
		close(job->pipefd[0]);
		redirect_fds(job);
		if (ft_strchr(job->cmd, '/'))
			cmd_path = file_path(job->cmd, job);
		else
			cmd_path = find_cmd_path(job->cmd, job);
		argv = get_argv(job);
		if (!argv)
			error_exit((free(cmd_path), "malloc error\n"), 1, job);
		envp = get_envp();
		if (!envp)
			error_exit((free((free_arrays(envp), cmd_path)), \
			"malloc error\n"), 1, job);
		execve(cmd_path, argv, envp);
		free((free((perror(job->cmd), cmd_path)), (free_arrays(envp), argv)));
		error_exit("", 126, job);
	}
}

int	exec_builtin(t_job *cmd_line, int fd, bool child)
{
	if (ft_strncmp("pwd", cmd_line->cmd, 4) == 0)
		ft_pwd(fd);
	else if (ft_strncmp("cd", cmd_line->cmd, 3) == 0)
		ft_cd(&cmd_line);
	else if (ft_strncmp("echo", cmd_line->cmd, 5) == 0)
		ft_echo(cmd_line, fd);
	else if (ft_strncmp("env", cmd_line->cmd, 4) == 0)
		ft_env(&cmd_line, fd);
	else if (ft_strncmp("export", cmd_line->cmd, 7) == 0)
		ft_export(&cmd_line, fd);
	else if (ft_strncmp("unset", cmd_line->cmd, 6) == 0)
	{
		g_exit_stat = 0;
		ft_unset(&cmd_line);
	}
	else if (ft_strncmp("exit", cmd_line->cmd, 5) == 0)
		ft_exit(&cmd_line, fd);
	if (child == TRUE)
	{
		error_exit("", 0, cmd_line);
		exit(g_exit_stat);
	}
	return (0);
}

void	builtin_child(t_job *job)
{
	pid_t	cpid;

	cpid = fork();
	if (cpid == 0)
	{
		close(job->pipefd[0]);
		redirect_fds(job);
		exec_builtin(job, 1, TRUE);
	}
}

void	execute_pipes(t_job *current)
{
	int		stat;
	int		n_child;
	int		i;

	stat = 0;
	n_child = 0;
	while (current)
	{
		if (pipe(current->pipefd) < 0)
			perror("pipe");
		if (current->cmd && check_builtin(current->cmd) && current->in != -1)
			builtin_child((n_child++, current));
		else if (current->cmd && current->in != -1)
			non_builtin_child((n_child++, current));
		close(current->pipefd[1]);
		if (current->prev)
			close(current->prev->pipefd[0]);
		current = current->next;
	}
	i = 0;
	while (i++ < n_child)
		wait(&stat);
	if (n_child)
		g_exit_stat = get_child_status(stat);
}

void	execute_jobs(t_job *jobs)
{
	int	outfd;

	if (!jobs)
		return ;
	if (check_builtin(jobs->cmd) && !jobs->next && jobs->in != -1)
	{
		outfd = 1;
		if (jobs->out == 3)
		outfd = open(jobs->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (jobs->out == 4)
		outfd = open(jobs->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
		exec_builtin(jobs, outfd, FALSE);
	}
	else
		execute_pipes(jobs);
}
