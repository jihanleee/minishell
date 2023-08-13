#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_token	*tokens;
	t_job	*jobs;
	char	*line;
	int		i;

	(void)argc;
	(void)argv;
	i = 0;
	while (envp[i])
		i++;
	if (!sigaction_set())
		write(STDERR_FILENO, "Error: sigaction_set_failed\n", 28);
	while (1)
	{
		line = readline(GREEN "MINISHELL> ");
		if (line == NULL)
		{
			while (envp[i])
				free(envp[i++]);
			write(STDOUT_FILENO, "exit\n", 5);
			return (0);
		}
		if (ft_strlen(line) == 0)
			continue ;
/* 		printf("----------------------------------\n");
		printf("line : %s\n", line);
		ft_printf("%d\n", g_exit_stat); */
		add_history(line);
		tokens = create_tokens(line);
		if (check_tokens(tokens) != 0)
			clear_tokens(&tokens, free);
		else
		{
			tokens = parse_tokens(&tokens, free);
			expansion(&tokens, envp);
/* 			read_tokens(tokens); */
			g_exit_stat = 0;
			open_file_redir(tokens);
			jobs = extract_jobs(tokens);
/* 			read_jobs(jobs); */
			execute_jobs(jobs, envp);
			clear_jobs(&jobs);
			clear_tokens(&tokens, free);
				//parsing error 있는 경우 이미 exit_error에서 clear_tokens를 함
				//main 정확히 짤 때는 두번 콜되지 않게 조심하기
		}
	}
	rl_clear_history();
	return (0);
}
