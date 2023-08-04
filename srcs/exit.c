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
static int	temp_exit(t_pipe **lst, char **env, int fd)
{
	t_pipe	*current;
	int error_type;

	current = *lst;
	//if (current->arg[1] != NULL)
	if (current->arg)
		//too many arguments error
		printf("exit - too many arguments error detected\n");	//error type = 1;
		//exit_error("exit: too many arguments\n", &current);
			//actually it shouldn't just print error message and exit
			//it's an error but the program shouldn't just end here
			//examples
				//exit 125 534 | echo hello
				//=>hello
				//=>exit: too many arguments (and doesn't exit)

				//echo hello | exit 124 5342
				//==>exit: too many arguments
		//hmm...we should probably return an int indicating an error
			//and save the error message somewhere? not sure about the printing order
	if ((check_int(current->arg)) == 0)	//what comes after 'exit' is not a number //depending on the flow, it may need to be 'else if'
		printf("error in exit detected\n");
		//wrong format error
			//prints "exit: (word after exit): numeric argument required"	//error type = 2;
	//free
	//finish
		//only exit ==> exit shell
		//there is another pipe after exit (ex: echo hello | exit 125 | cat -e)
			//nothing happens (exit 125 probably ends the subshell, not the actual shell, and moves to the next pipe)
}
*/

/*
int main(int argc, char **argv)
{
	(void)argc;
	printf("result is %d\n", check_int(argv[1]));
}
*/