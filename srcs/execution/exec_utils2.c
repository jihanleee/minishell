/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jihalee <jihalee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 21:49:33 by jihalee           #+#    #+#             */
/*   Updated: 2023/10/21 13:53:33 by jihalee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_builtin(char *cmd)
{
	if (cmd == NULL)
		return (0);
	if (!ft_strncmp(cmd, "cd", 3) || !ft_strncmp(cmd, "echo", 5)
		|| !ft_strncmp(cmd, "pwd", 4) || !ft_strncmp(cmd, "env", 4)
		|| !ft_strncmp(cmd, "export", 7) || !ft_strncmp(cmd, "export", 7)
		|| !ft_strncmp(cmd, "unset", 6) || !ft_strncmp(cmd, "exit", 5))
	{
		return (1);
	}
	return (0);
}

char	**get_envp(void)
{
	t_env	*env;
	t_env	*fst;
	int		i;
	char	**result;

	i = 0;
	env = *get_env_address();
	fst = env;
	while (env)
	{
		env = env->next;
		i++;
	}
	result = (char **)ft_calloc(i + 1, sizeof (char *));
	i = 0;
	while (fst)
	{
		result[i] = ft_strdup(fst->str);
		fst = fst->next;
		i++;
	}
	return (result);
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
	result = (char **)ft_calloc(size + 1, sizeof(char *));
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
	else
		outfd = 1;
	if (outfd == -1 || infd == -1)
		error_exit("", 1, job);
	if (dup2(outfd, 1) < 0 || dup2(infd, 0) < 0)
		error_exit("dup2 error\n", 1, job);
	return (0);
}

int	get_child_status(int stat)
{
	if (WIFEXITED(stat))
		return (WEXITSTATUS(stat));
	else if (WIFSIGNALED(stat))
		return (WTERMSIG(stat) + 128);
	return (0);
}
