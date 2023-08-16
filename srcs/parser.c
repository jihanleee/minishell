#include "minishell.h"

void	temp_read_tokens(t_token **lst)
{

	t_token	*current;

	current = *lst;
	while (current)
	{
		current = current->next;
	}
}

void	change_type(t_token *current, t_token *next)

{
	if (next->type != 0 && next->type != 5)
	{
		next->next->type = next->type;
		current->next = next->next;
		next->next = NULL;
		free(next->str);
		free(next);
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
	if (current->type != 0)
	{
		temp = current;
		current->next->type = current->type;
		current = current->next;
		result = result->next;
		temp->next = NULL;
		free(temp->str);
		free(temp);
	}
	while (current && current->next)
	{
		next = current->next;
		change_type(current, next);
		current = current->next;
	}
	return (result);
}

void	print_parse_error(t_token **input)
{
	t_token	*current;

	g_exit_stat = 2;
	current = *input;
	write(2, "bash: syntax error near unexpected token \'", 42);
	if (current->type >= 1 && current->type <= 4)
	{
		if (!current->next)
			write(2, "newline", 7);
	}
	else
		write(2, current->str, get_length(current->str));
	if (ft_strncmp(current->str, "|", 1) == 0 && current->next)
	{
		current = current->next;
		if (ft_strncmp(current->str, "|", 1) == 0)
			write(2, "|", 1);
	}
	write(2, "\'", 1);
	write(2, "\n", 1);
}

int	token_error_check(t_token *tokens)
{

	if (tokens->type == 5 && tokens->next && tokens->next->type == 5)
	{
		print_parse_error(&tokens);
		return (1);
	}
	if (tokens->type != 0 && tokens->type != 5)
	{
		if (tokens->next == NULL || tokens->next->type != 0)
		{
			print_parse_error(&tokens);
			return (1);
		}
	}
	if (tokens->type == pipe_op && tokens->next && tokens->next->type == pipe_op)
	{
		print_parse_error(&tokens);
		return (1);
	}
}

int	check_tokens(t_token *tokens)
{
	if (!tokens)
		return (0);
	if (tokens->type == 5)
	{
		print_parse_error(&tokens);
		return (1);
	}
	while (tokens && tokens->str)
	{
		token_error_check(tokens);
		tokens = tokens->next;
	}
	return (0);
}
