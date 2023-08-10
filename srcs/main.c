#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_token	*tokens;
	t_job	*jobs;
	//t_list *history;
	//t_list new;
	//t_token	*temp;
	char	*line;
	//int		*info;
	//int		i;
	//char	*tok;

	(void)argc;
	(void)argv;
	if (!sigaction_set())
		write(STDERR_FILENO, "Error: sigaction_set_failed\n", 28);
	while (1)
	{	//temp = &tokens;
		line = readline(GREEN "MINISHELL> ");
		if (line == NULL)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			return (0);
		}
		if (ft_strlen(line) == 0)
			continue ;
		printf("----------------------------------\n");
		printf("line : %s\n", line);
		ft_printf("%d\n", g_exit_stat);
		add_history(line);
		tokens = create_tokens(line);
		//printf("check tokens result is %d\n", check_tokens(tokens));
		if (check_tokens(tokens) != 0)
		{
			//exit_error("bash: syntax error\n", &tokens);
			//printf("bash: syntax error\n");
			//print_error("syntax error\n");
			clear_tokens(&tokens, free);
		}
		else
		{
			tokens = parse_tokens(&tokens, free);
			expansion(&tokens, envp);
			g_exit_stat = 0;
			open_file_redir(tokens);
			jobs = extract_jobs(tokens);

			read_jobs(jobs);
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

/*
void	clear_tokens(t_token **lst, void (*del)(void *))
{
	t_token	*current;
	t_token	*next;

	if (lst == 0)
		return ;
	if (del == 0)
		return ;
	current = *lst;
	ft_printf("currently in clear tokens function\n"); //
	while (current)
	{
		ft_printf("\ttoken we are deleting is %s\n", current->str);//
		next = current->next;
		if (current->str)
			del(current->str);
		free(current);
		current = next;
	}
	*lst = 0;
}
*/

/*
int	main(int argc, char **argv, char **envp)
{
	t_token	*tokens;
	//t_token	*temp;
	char	*line;
	//int		*info;
	//int		i;
	//char	*tok;

	if (!sigaction_set())
		printf("\n Error: sigaction_set_failed\n");
	while (1)
	{	//temp = &tokens;
		line = readline("%");
		ft_printf("line : %s\n", line);
		tokens = create_tokens(line);
		printf("\n-----tokenize done-----\n");
		printf("\n-----parsing starts-----\n");
		temp_read_tokens(&tokens);
		if (check_tokens(tokens) != 0)
		{
			exit_error("bash: syntax error\n", &tokens);
		}
		else
		{
			tokens = parse_tokens(&tokens, free);
			printf("\ntokens after parsing\n");
			temp_read_tokens(&tokens); //
		}
		//expansion
		clear_tokens(&tokens, free);
			//parsing error 있는 경우 이미 exit_error에서 clear_tokens를 함
			//main 정확히 짤 때는 두번 콜되지 않게 조심하기
			//because we send new list from expansion to execution, we need to free the 
				//previous token list
		//execution
	}
	return (0);
}
*/


//token type
//0: word
//1: <
//2: heredoc
//3: >
//4: append
//5: pipe
