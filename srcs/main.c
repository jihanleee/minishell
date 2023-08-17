#include "minishell.h"

int	g_exit_stat = 0;

t_token	**get_token_address(t_token *new)
{
	static t_token	*addr;

	if (!addr)
		addr = new;
	return (&addr);
}

int	main(int argc, char **argv, char **envp)
{
	t_token	*tokens;
	t_job	*jobs;
	char	*line;

	g_exit_stat = 0;
	init_env_var(envp);
	(void)argc;
	(void)argv;
	tokens = 0;
	while (1)
	{
		if (!sigaction_set_prompt())
			write(2, "Error: sigaction_set_failed\n", 28);
		line = readline(GREEN "MINISHELL> " BACK);
		if (line == NULL)
		{
			clear_env();
			rl_clear_history();
			write(1, "exit\n", 5);
			return (0);
		}
		if (ft_strlen(line) == 0)
			continue ;
		add_history(line);
		tokens = create_tokens(line);
		if (check_tokens(tokens) != 0)
			clear_tokens(&tokens, free);
		else
		{
			tokens = parse_tokens(&tokens, free);
			expansion(&tokens);
			get_token_address(tokens);
			open_file_redir(get_token_address(0));
			jobs = extract_jobs(*get_token_address(0));
			clear_tokens(get_token_address(0), free);
			sigaction_set_parent();
			execute_jobs(jobs);
			clear_jobs(jobs);
		}
	}
	return (0);
}
