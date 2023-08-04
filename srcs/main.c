#include "minishell.h"


int	main(int argc, char **argv, char **envp)
{
	t_token	*tokens;
	t_pipe	*pipes;
	//t_list *history;
	//t_list new;
	//t_token	*temp;
	char	*line;
	//int		*info;
	//int		i;
	//char	*tok;

	(void)argc;
	(void)argv;
	while (1)
	{	//temp = &tokens;
		line = readline("%");
		printf("----------------------------------\n");
		printf("line : %s\n", line);
		add_history(line);
		tokens = create_tokens(line);
		//printf("check tokens result is %d\n", check_tokens(tokens));
		if (check_tokens(tokens) != 0)
		{
			//exit_error("bash: syntax error\n", &tokens);
			printf("bash: syntax error\n");
			clear_tokens(&tokens, free);
			return (1);	//error
		}
		else
		{
			tokens = parse_tokens(&tokens, free);
		}
		expansion(&tokens, envp);
		read_tokens(tokens);
		printf("read tokens done\n");
		open_file_redir(tokens);
		pipes = extract_pipes(tokens);
		read_pipes(pipes);
		printf("read pipes done\n");
		//test_execute(pipes, envp, 1);
		//exec_command_line(t_pipe *pipe, char **env);
		exec_command_line(&pipes, envp);
		clear_tokens(&tokens, free);
			//parsing error 있는 경우 이미 exit_error에서 clear_tokens를 함
			//main 정확히 짤 때는 두번 콜되지 않게 조심하기
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
