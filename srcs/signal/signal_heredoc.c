#include "minishell.h"

int	sigaction_set_heredoc(void)
{
	static struct sigaction	sig_struct;

	sigemptyset(&sig_struct.sa_mask);
	sig_struct.sa_flags = SA_SIGINFO;
	sig_struct.sa_sigaction = &sig_handler_heredoc;
	if (sigaction(SIGINT, &sig_struct, 0) == -1)
		return (0);
	if (sigaction(SIGQUIT, &sig_struct, 0) == -1)
		return (0);
	return (1);
}

void	sig_handler_heredoc(int signum, siginfo_t *info, void *context)
{
	(void)info;
	(void)context;
	if (signum == SIGINT)
	{
		clear_tokens(get_token_address(0), free);
		rl_clear_history();
		clear_env();
		exit(130);
	}
	else if (signum == SIGQUIT)
		return ;
	return ;
}
