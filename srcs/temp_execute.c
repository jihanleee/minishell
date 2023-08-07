#include "minishell.h"

/*
static void	exit_error_message(t_job *jobs, int exit, int fd)
{
	if (exit / 10 == 1)
	{
		//write(fd, "exit\n", 5);
		write(fd, "bash: exit: too many arguments\n", 31);
	}
	else if (exit / 10 == 2)
	{
		//write(fd, "exit\n", 5);
		write(fd, "bash: exit: ", 12);
		write(fd, &jobs->arg[0], get_length(jobs->arg[0]);
		write(1, ": numeric argument required\n", 28);
	}
}
*/

void	test_execute(t_job *jobs, char **env, int fd)
{
	printf("in test execute\n");
	while(jobs)
	{
		if (ft_strncmp(jobs->cmd, "echo", 4) == 0 && jobs->cmd[4] == 0)
		{
			//ft_printf("inside echo\n");
			ft_echo(&jobs, env, 1);	//change 1 to fd
		}
		else if (ft_strncmp(jobs->cmd, "env", 3) == 0 && jobs->cmd[3] == 0)
			ft_env(&jobs, env, 1);	//change 1 to fd
		else if (ft_strncmp(jobs->cmd, "export", 6) == 0 && jobs->cmd[6] == 0)
			ft_export(&jobs, env, 1);	//change 1 to fd
		else if (ft_strncmp(jobs->cmd, "pwd", 3) == 0 && jobs->cmd[3] == 0)
			ft_pwd(&jobs, env, 1);	//change 1 to fd
		else if (ft_strncmp(jobs->cmd, "unset", 5) == 0 && jobs->cmd[5] == 0)
			ft_unset(&jobs, env, 1);	//change 1 to fd
		//else if (ft_strncmp(jobs->cmd, "exit", 4) == 0)
		//	ft_exit(&jobs, env, 1);	//change 1 to fd
		else if (ft_strncmp(jobs->cmd, "cd", 2) == 0 && jobs->cmd[2] == 0)
			ft_cd(&jobs, env, 1);	//change 1 to fd
		jobs = jobs->next;
	}
}