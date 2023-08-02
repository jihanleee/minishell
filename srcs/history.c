#include "minishell.h"

t_list *ft_lstnew(void *content)
{
	t_list *list;

	list = malloc(sizeof(t_list));
	if (!list)
		return (NULL);
	return(list);
}