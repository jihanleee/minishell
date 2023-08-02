#include "minishell.h"

/*
static void	exit_error_message(t_pipe *pipes, int exit, int fd)
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
		write(fd, &pipes->arg[0], get_length(pipes->arg[0]);
		write(1, ": numeric argument required\n", 28);
	}
}
*/

void	test_execute(t_pipe *pipes, char **env, int fd)
{
	printf("in test execute\n");
	while(pipes)
	{
		if (ft_strncmp(pipes->cmd, "echo", 4) == 0 && pipes->cmd[4] == 0)
		{
			//ft_printf("inside echo\n");
			ft_echo(&pipes, env, 1);	//change 1 to fd
		}
		else if (ft_strncmp(pipes->cmd, "env", 3) == 0 && pipes->cmd[3] == 0)
			ft_env(&pipes, env, 1);	//change 1 to fd
		else if (ft_strncmp(pipes->cmd, "export", 6) == 0 && pipes->cmd[6] == 0)
			ft_export(&pipes, env, 1);	//change 1 to fd
		else if (ft_strncmp(pipes->cmd, "pwd", 3) == 0 && pipes->cmd[3] == 0)
			ft_pwd(&pipes, env, 1);	//change 1 to fd
		else if (ft_strncmp(pipes->cmd, "unset", 5) == 0 && pipes->cmd[5] == 0)
			ft_unset(&pipes, env, 1);	//change 1 to fd
		//else if (ft_strncmp(pipes->cmd, "exit", 4) == 0)
		//	ft_exit(&pipes, env, 1);	//change 1 to fd
		else if (ft_strncmp(pipes->cmd, "cd", 2) == 0 && pipes->cmd[2] == 0)
			ft_cd(&pipes, env, 1);	//change 1 to fd
		pipes = pipes->next;
	}
}