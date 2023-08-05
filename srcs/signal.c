#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "../includes/minishell.h"

void	new_prompt(void)
{
 //printing ^C
 //	write(STDOUT_FILENO, "^C\n", 3);
 //new prompt

	rl_replace_line("", 1);
 	if (rl_on_new_line() != 0)
		exit (1);
	write(1, "\n", 1);
 	rl_redisplay();
	/*TEST EXIT*/
	//exit (0);
}

void sig_handler(int signum, siginfo_t *info, void *context)
{
	//static int start = 0;
	(void)info;
	(void)context;
	if (signum == SIGINT) //Ctrl + C -> new prompt , new line
	{
		// save 130 in global variable when interupted. global variable to be made by execution part.
		new_prompt();
	}
	else if (signum == SIGQUIT) //ctrl + \ -> nothing happens
		return ;
	return ;
}

int sigaction_set(void)
{
	struct sigaction sig_struct;

	sigemptyset(&sig_struct.sa_mask);
	sig_struct.sa_flags = SA_SIGINFO;
	sig_struct.sa_sigaction = &sig_handler;
	if (sigaction(SIGINT, &sig_struct, NULL) == -1) // Ctrl + C sends SIGINT(=2)
		return (0);
	else if (signal(SIGQUIT, SIG_IGN) == -1)
		;
	return (1);
}
