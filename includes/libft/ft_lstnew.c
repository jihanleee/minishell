/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jihalee <jihalee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/30 15:30:43 by jihalee           #+#    #+#             */
/*   Updated: 2023/05/01 04:15:57 by jihalee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"
#include <stdlib.h>

/*
t_list	*ft_lstnew(char *content)
{
	t_list	*new;

	new = (t_list *)malloc(sizeof (t_list));
	if (new == 0)
		return (NULL);
	new->next = 0;
	new->line = content;
	return (new);
}
*/

t_list *ft_lstnew(void *content)
{
	t_list *list;

	list = malloc(sizeof(t_list));
	if (!list)
		return (NULL);
	list->content = content;
	list->next = NULL;
	return (list);
}