#include "minishell.h"

/* 업데이트!!!!!!!
bash shell 에서 exit 256 넘어가면 256의 나머지 값으로 exit status 저장됨. 
256배수로 exit 해보면 exit status 0 으로 나옴
exit 함수 조건문 약간 바꿈
*/
int	g_exit_stat;

int	check_int(char *arg)
{
	int	i;

	i = 0;
	if (arg == NULL)
		return (1);
	if (arg[i] == '-')
		i++;
	while (arg[i])
	{
		if (arg[i] < '0' || arg[i] > '9')
			return (1);
		i++;
	}
	return (0);
}

int	cnt_arg(t_job *current)
{
	int	cnt;

	cnt = 0;
	if (!current->arg)
		return (cnt);
	while (current->arg[cnt])
		cnt++;
	return (cnt);
}
void	free_exit(int exit_status, t_job *job)
{
	rl_clear_history();
	clear_env();
	clear_jobs(job);
	exit(exit_status);
}

void	exit_error_cases(int arg_cnt, t_job *current)
{
	if (arg_cnt > 1 && check_int(current->arg[0]) == 0) // exit with too many arguments
	{
		write(2, "exit\n", 5);
		write(2, "bash: exit: too many arguments\n", 31);
		g_exit_stat = 1; //error
		free_exit(1, current);
	}
	else // arg_cnt 1 이상이고, int 아닐 때, arg_cnt 1개이고 int 아닐때 pigone hada
	{
		write(2, "bash: exit: ", 12);
		write(2, current->arg[0], ft_strlen(current->arg[0]));
		write(2, ": numeric argument required\n", 28);
		g_exit_stat = 2;
	}
}
int	ft_exit(t_job **lst, int fd)
{
	t_job	*current;
	int		exit_num;
	int		arg_cnt;

	exit_num = 0;
	current = *lst;
	arg_cnt = cnt_arg(current);

	if (current->next)
		return (0); // If there's a pipe after exit cmd, the execution stops here!!!
	if (arg_cnt == 0)
		free_exit(0, current); // If pipe or arg not found, exit(0)
	if (arg_cnt == 1 && check_int(current->arg[0]) == 0)  //exit 1234564
	{
		exit_num = atoi(current->arg[0]) % 256;
		write(fd, "exit\n", 5);
		g_exit_stat = exit_num;
		if (exit_num == 0)
			return (0);	//only case where it's true
		free_exit(exit_num, current);
	}
	exit_error_cases(arg_cnt, current);
	return (-1); //if not 0 or 125, 
}
