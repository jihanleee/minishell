

#include "minishell.h"

/* 업데이트!!!!!!!
bash shell 에서 exit 256 넘어가면 256의 나머지 값으로 exit status 저장됨. 
256배수로 exit 해보면 exit status 0 으로 나옴
exit 함수 조건문 약간 바꿈
*/


int	check_int(char *arg)
{
	int	i;

	i = 0;
	printf("inside check int\n");
	printf("current arg is %s\n", arg);
	if (arg == NULL)
		return (1);
	if (arg[i] == '-')
		i++;
	while (arg[i])
	{
		printf("current char: %c and has ascii %d\n", arg[i], arg[i]);
		if (arg[i] < '0' || arg[i] > '9')
			return (1);
		i++;
	}
	return (0);
}

/*
int	get_exit_num(t_pipe current)
{
	int	result;
	int	i;

	result = 0;
	i = 0;
	while (current.arg[0][i])
	{
		printf("current char was %c\n", current.arg[0][i]);
		result = (result * 10) + (current.arg[0][i] - '0');
		printf("current result is %d\n", result);
		i++;
	}
	while (result > 255)
		result = result - 255;
	printf("\t\tfinal exit number is %d\n", result);
	return (result);
}
*/

int	cnt_arg(t_pipe current)
{
	int	cnt;

	cnt = 0;
	while (current.arg[cnt])
		cnt++;
	return (cnt);
}

int	ft_exit(t_pipe **lst, char **env, int fd)
{
	t_pipe	*current;
	int		exit_num;
	int		arg_cnt;

	(void)env;
	exit_num = 0;
	current = *lst;

	if (current->next)
		return (0); // If there's a pipe after exit cmd, the execution stops here!!!

	if (arg_cnt == 0)
		exit(0); // If pipe or arg not found, exit(0)

	else if (arg_cnt == 1 && check_int(current->arg[0]) == 0)  //exit 1234564
	{
		exit_num = ft_atoi(current->arg[0]) % 256;
		write(fd, "exit\n", 5);
		g_exit_stat = exit_num;
		exit(exit_num);
	}

	else if (arg_cnt > 1 && check_int(current->arg[0]) == 0) // exit with too many arguments
	{
		write(fd, "exit\n", 5);
		write(fd, "bash: exit: too many arguments\n", 31);
		g_exit_stat = 1; //error
		return (-1);
	}
	else // arg_cnt 1 이상이고, int 아닐 때, arg_cnt 1개이고 int 아닐때 pigone hada
	{
		write(fd, "bash: exit: ", 12);
		write(fd, current->arg[0], get_length(current->arg[0]));
		write(fd, ": numeric argument required\n", 28);
		g_exit_stat = 255;
		return (-1);
	}
	return (-1); //if not 0 or 125, 
}




/* 조건문 하나하나 다 쓴 코드
int	ft_exit(t_pipe **lst, char **env, int fd)
{
	t_pipe	*current;
	int		exit_num;
	int		arg_cnt;

	(void)env;
	exit_num = 0;
	current = *lst;
	arg_cnt = cnt_arg(*current);

	if (current->next)
		return ; // If there's a pipe after exit cmd, the execution stops here!!!

	if (arg_cnt == 0)
		exit(0); // If pipe or arg not found, exit(0)

	else if (arg_cnt == 1)
	{
		if (check_int(current->arg[0]) == 0)
		{
			exit_num = ft_atoi(current->arg[0]);
			exit_num %= 256; // Adjust exit status to be within 0 to 255
			write(fd, "exit\n", 5);
			g_exit_stat = exit_num;
			exit(exit_num);
		}
		else
		{
			write(fd, "bash: exit: ", 12);
			write(fd, current->arg[0], get_length(current->arg[0]));
			write(fd, ": numeric argument required\n", 28);
			g_exit_stat = 255;
			return (-1);
		}
	}

	else if (arg_cnt > 1)
	{
		if (check_int(current->arg[0]) == 0)
		{
			write(fd, "exit\n", 5);
			write(fd, "bash: exit: too many arguments\n", 31);
			g_exit_stat = 1; //error
		}
		else
		{
			write(fd, "bash: exit: ", 12);
			write(fd, current->arg[0], get_length(current->arg[0]));
			write(fd, ": numeric argument required\n", 28);
			g_exit_stat = 255;
		}
		return (-1);
	}

	return (-1); //if not 0 or 125, 
}
*/



/*원래 코드



#include "minishell.h"

int	check_int(char *arg)
{
	int	i;

	i = 0;
	printf("inside check int\n");
	printf("current arg is %s\n", arg);
	if (arg == NULL)
		return (1);
	if (arg[i] == '-')
		i++;
	while (arg[i])
	{
		printf("current char: %c and has ascii %d\n", arg[i], arg[i]);
		if (arg[i] < '0' || arg[i] > '9')
			return (1);
		i++;
	}
	return (0);
}

int	get_exit_num(t_pipe current)
{
	int	result;
	int	i;

	result = 0;
	i = 0;
	while(current.arg[0][i])
	{
		printf("current char was %c\n", current.arg[0][i]);
		result = (result * 10) + (current.arg[0][i] - '0');
		printf("current result is %d\n", result);
		i++;
	}
	while (result > 255)
		result = result - 255;
	printf("\t\tfinal exit number is %d\n", result);
	return (result);
}


int	ft_exit(t_pipe **lst, char **env, int fd)
{
	t_pipe	*current;
	int		exit_num;

	(void)env;
	current = *lst;
	if (current->arg[1])	//too many arguments error
	{
		write(fd, "exit\n", 5);
		write(fd, "bash: exit: too many arguments\n", 31);
		return (-1);
	}
	else if ((check_int(current->arg[0])) == 1)	//what comes after 'exit' is not a number //depending on the flow, it may need to be 'else if'
	{
		//bash: exit: hi: numeric argument required
		write(fd, "bash: exit: ", 12);
		write(fd, current->arg[0], get_length(current->arg[0]));
		write(fd, ": numeric argument required\n", 28);
		return (-1);
	}
	else
	{
		exit_num = get_exit_num(*current);
		//add code
		write(fd, "exit\n", 5);
	}
	if (exit_num == 0)
		return (0);	//only case where it's true
	return (-1); //if not 0 or 125, 
}

/*


*/
