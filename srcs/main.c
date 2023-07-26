#include "minishell.h"
void	clear_tokens(t_token **lst, void (*del)(void *))
{
	t_token	*current;
	t_token	*next;

	if (lst == 0)
		return ;
	if (del == 0)
		return ;
	current = *lst;
	printf("currently in clear tokens function\n"); //
	while (current)
	{
		printf("\ttoken we are deleting is %s\n", current->str);//
		next = current->next;
		if (current->str)
			del(current->str);
		free(current);
		current = next;
	}
	*lst = 0;
}


int	main(int argc, char **argv, char **envp)
{
	t_token	*tokens;
	//t_token	*temp;
	char	*line;
	//int		*info;
	//int		i;
	//char	*tok;

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
		//execution
		clear_tokens(&tokens, free);
	}
	return (0);
}

//token type
//0: word
//1: <
//2: heredoc
//3: >
//4: append
//5: pipe