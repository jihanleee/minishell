#include "minishell.h"

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
