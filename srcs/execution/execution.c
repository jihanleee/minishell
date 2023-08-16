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

int	exec_builtin(t_job *cmd_line, int fd)
{
	g_exit_stat = 0;
	if (ft_strncmp("pwd", cmd_line->cmd, 4) == 0)
		ft_pwd(fd);
	else if (ft_strncmp("cd", cmd_line->cmd, 3) == 0)
		ft_cd(&cmd_line, fd);
	else if (ft_strncmp("echo", cmd_line->cmd, 5) == 0)
		ft_echo(cmd_line, fd);
	else if (ft_strncmp("env", cmd_line->cmd, 4) == 0)
		ft_env(&cmd_line, fd);
	else if (ft_strncmp("export", cmd_line->cmd, 7) == 0)
		ft_export(&cmd_line, fd);
	else if (ft_strncmp("unset", cmd_line->cmd, 6) == 0)
		ft_unset(&cmd_line);
	else if (ft_strncmp("exit", cmd_line->cmd, 5) == 0)
		ft_exit(&cmd_line, fd);
	return (0);
}

void	builtin(t_job *job)
{
	int		outfd;

	if (job->out == 3)
		outfd = open(job->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (job->out == 4)
		outfd = open(job->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (job->next && job->out == 0)
		outfd = job->pipefd[1];
	else if (!job->next)
		outfd = 1;
	exec_builtin(job, outfd);
}

int	check_builtin(char *cmd)
{
	if (!ft_strncmp(cmd, "cd", 3) || !ft_strncmp(cmd, "echo", 5)
		|| !ft_strncmp(cmd, "pwd", 4) || !ft_strncmp(cmd, "env", 4)
		|| !ft_strncmp(cmd, "export", 7) || !ft_strncmp(cmd, "export", 7)
		|| !ft_strncmp(cmd, "unset", 6) || !ft_strncmp(cmd, "exit", 5))
	{
		return (1);
	}
	return (0);
}

void	execute_jobs(t_job *current)
{
	int		stat;
	int		n_child;
	int		i;
	int		r_wait;

	stat = 0;
	n_child = 0;
	while (current)
	{
		if (pipe(current->pipefd) < 0)
			perror("pipe");
		if (current->cmd && check_builtin(current->cmd))
			builtin(current);
		else if (current->cmd)
			non_builtin_child((n_child++, current));
		close(current->pipefd[1]);
		if (current->prev)
			close(current->prev->pipefd[0]);
		current = current->next;
	}
	i = 0;
	while (i++ < n_child)
		r_wait = wait(0);
	if (n_child && r_wait == 0)
		g_exit_stat = get_child_status(stat);
}
