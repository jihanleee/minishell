/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_free.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jihalee <jihalee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 21:49:16 by jihalee           #+#    #+#             */
/*   Updated: 2023/09/26 23:04:42 by jihalee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_pipes(t_job *current)
{
	close(current->pipefd[1]);
	if (current->prev)
		close(current->prev->pipefd[0]);
	if (!current->next)
		close(current->pipefd[0]);
}

void	free_empty_cmd(char *cmd_path, t_job *job)
{
	if (job->cmd[0] == '\0')
	{
		free(cmd_path);
		error_exit((ft_putstr_fd(job->cmd, 2), \
		": command not found\n"), 127, job);
	}
}

void	free_arrays(char **str)
{
	int	i;

	i = 0;
	while (str[i])
		free(str[i++]);
	free(str);
}
