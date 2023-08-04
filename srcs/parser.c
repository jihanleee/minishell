#include "minishell.h"

/*
parsing rules

if token.redirection == 1 (meaning there is redirection)
	and **files is empty ==> error
if token redirection is <
	and *file[0] is empty ==> error
if token redirection is >
	and *file[1] is empty ==> error

if token after pipe is not a word, redirection etc...
=if token after pipe is a pipe ==> error

*/
//receive tokens list
//go through each token
	//if redirection == 1 and **files are empty ==> error
	//if redirection is < and files[1] is empty ==> error
	// if redirection is > and files[2] is empty ==> error
	//if pipe is followed by a pipe ==> error
	//no file(word in this case) after redirections


void	temp_read_tokens(t_token **lst)
{

	t_token	*current;

	current = *lst;
	printf("currently in temp read function\n"); //
	while (current)
	{
		printf("\tcurrent token has %s with type %d\n", current->str, current->type);
		current = current->next;
	}
}


t_token	*parse_tokens(t_token **lst, void (*del)(void *))
{
	t_token	*current;
	t_token	*result;
	t_token	*temp;
	t_token	*next;

	if (lst == 0 || del == 0 || *lst == 0)
		return (NULL);
	result = *lst;
	current = *lst;
	printf("-------------------------------------\n");
	printf("\ncurrently in parse tokens function\n"); //
	if (current->type != 0) //&& current->type != 5
	{
		temp = current;
		current->next->type = current->type;
		current = current->next;
		result = result->next;
		temp->next = NULL;
		free(temp);
	}
	while (current && current->next)
	{
		printf("\tcurrent token is %s\n", current->str);//
		next = current->next;
		if (next->type != 0 && next->type != 5)
		{
			next->next->type = next->type;
			current->next = next->next;
			next->next = NULL;
			free(next);
		}
		current = current->next;
	}
	return (result);
}


void	exit_error(char *message, t_token **lst)
{
	write(STDERR_FILENO, message, ft_strlen(message));
	//temp_read_tokens(temp);
	//parse_clear_tokens(temp, free);
	clear_tokens(lst, free);
}

int	check_tokens(t_token *tokens)
{
	int	result;

	result = 0;
	if (!result)
		return (0);
	printf("\nin check tokens function\n");//
	//printf("\ttype right now is %d\n", tokens->type);
	if (tokens->type == 5)	//if the first token is a pipe
	{
		//result = 1;
		return (1);
		//tokens = tokens->next;
	}
	while (tokens && tokens->str)
	{
		printf("\tcurrent token is %s\n", tokens->str);//
		if (tokens->type != 0 && tokens->type != 5)
		{
			if (tokens->next == NULL || tokens->next->type != 0)
				//when line ends with <, <<, >, >>
				//result = 1;
				return (1);
		}
		//if (tokens->type == in || tokens->type == out)
		//{
		//	if (tokens->next->type != word)
		//		result = 1;
		//}
		if (tokens->type == pipe_op && tokens->next && tokens->next->type == pipe_op)
			//result = 1;
			return (1);
		tokens = tokens->next;
	}
	//printf("result is %d\n", result);
	//return (result);
	return (0);
}