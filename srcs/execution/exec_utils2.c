#include "minishell.h"

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

int	redirect_fds(t_job *job)
{
	int	infd;
	int	outfd;

	if (job->in)
		infd = open(job->infile, O_RDONLY);
	else if (job->prev && job->in == 0)
		infd = job->prev->pipefd[0];
	else
		infd = 0;
	if (job->out == 3)
		outfd = open(job->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (job->out == 4)
		outfd = open(job->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (job->next && job->out == 0)
		outfd = job->pipefd[1];
	else if (!job->next)
		outfd = 1;
	if (outfd == -1 || infd == -1)
		error_exit("fd error\n", 1);
	if (dup2(outfd, 1) < 0 || dup2(infd, 0) < 0)
		error_exit("dup2 error\n", 1);
	return (0);
}
