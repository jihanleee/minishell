#include <stdio.h>

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

int	ft_exit(t_pipe **lst, char **env)
{
	t_pipe	*current;

	current = *lst;
	if (current->arg->next != NULL)
		//too many arguments error
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
		//wrong format error
			//prints "exit: (word after exit): numeric argument required"
	//free
	//finish
		//only exit ==> exit shell
		//there is another pipe after exit (ex: echo hello | exit 125 | cat -e)
			//nothing happens (exit 125 probably ends the subshell, not the actual shell, and moves to the next pipe)
}

/*
int main(int argc, char **argv)
{
	(void)argc;
	printf("result is %d\n", check_int(argv[1]));
}
*/