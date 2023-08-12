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
	printf("\n-----------------------------------\n");
	printf("currently in clear tokens function\n");
	while (current)
	{
		printf("\ttoken we are deleting is %s\n", current->str);
		next = current->next;
		if (current->str)
			del(current->str);
		free(current);
		current = next;
	}
	*lst = 0;
}

void	clear_lexemes(t_lexeme **lst, void (*del)(void *))
{
	t_lexeme	*current;
	t_lexeme	*next;

	if (lst == 0)
		return ;
	if (del == 0)
		return ;
	current = *lst;
	while (current)
	{
		next = current->next;
		if (current->str)
			del(current->str);
		free(current);
		current = next;
	}
	*lst = 0;
}