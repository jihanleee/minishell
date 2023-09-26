/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jihalee <jihalee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 21:49:23 by jihalee           #+#    #+#             */
/*   Updated: 2023/09/26 23:42:19 by jihalee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	error_exit(char *str, int exit_status, t_job *job)
{
	close_pipes(job);
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
		path = 0;
	else
	{
		i = 0;
		while (env->str[i] && env->str[i] != '=')
			i++;
		i++;
		path = ft_split(&(env->str[i]), ':');
	}
	if (path == 0)
		error_exit((write(2, job->cmd, ft_strlen(job->cmd)), \
		": No such file or directory\n"), 127, job);
	return (path);
}

char	*file_path(char *cmd, t_job *job)
{
	DIR	*dir;

	dir = opendir(cmd);
	if (dir)
	{
		closedir(dir);
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": Is a directory\n", 2);
		error_exit("", 126, job);
	}
	if (access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	else
	{
		perror(cmd);
		if (errno == 13)
			error_exit("", 126, job);
		error_exit("", 127, job);
	}
	return (0);
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
		slashed = ft_strjoin(path[i++], "/");
		withcmd = ft_strjoin(slashed, cmd);
		if (slashed)
			free(slashed);
		if (withcmd == 0)
			return (NULL);
		if (access(withcmd, X_OK) == 0)
			break ;
		free(withcmd);
		withcmd = 0;
	}
	free_arrays(path);
	free_empty_cmd(withcmd, job);
	if (!withcmd)
		error_exit((ft_putstr_fd(cmd, 2), ": command not found\n"), 127, job);
	return (withcmd);
}
