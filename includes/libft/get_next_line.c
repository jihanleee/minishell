/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jihalee <jihalee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 15:50:05 by jihalee           #+#    #+#             */
/*   Updated: 2023/07/17 16:59:40 by jihalee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	find_nl(char *buf, int *i, int *j, int *eol_found)
{
	*i = *j;
	while (buf[*i] && buf[*i] != '\n')
		(*i)++;
	if (buf[*i] == '\n')
	{
		*eol_found = 1;
		(*i)++;
	}
}

int	add_to_list(char *buf, t_list_gnl **list, t_list_gnl **current)
{
	int			i;
	int			j;
	int			eol_found;
	t_list_gnl	*new;

	eol_found = 0;
	j = 0;
	while (buf[j])
	{
		find_nl(buf, &i, &j, &eol_found);
		new = gnl_lstnew(gnl_substr(buf, j, i - j), i - j);
		if (new == 0)
			return (-1);
		if (*list == 0)
		{
			*list = new;
			(*current) = *list;
		}
		else
			(*current)->next = new;
		if ((*current)->next)
			(*current) = (*current)->next;
		j = i;
	}
	return (eol_found);
}

int	make_list(int fd, t_list_gnl **list)
{
	char		*buf;
	int			size_read;
	t_list_gnl	*current;
	int			r_addtolist;

	r_addtolist = 0;
	buf = (char *)malloc(sizeof (char) * (BUFFER_SIZE + 1));
	if (buf == 0)
		return (0);
	current = gnl_lstlast(*list);
	while (r_addtolist == 0)
	{
		size_read = read(fd, buf, BUFFER_SIZE);
		if (size_read <= 0)
			break ;
		buf[size_read] = '\0';
		r_addtolist = add_to_list(buf, list, &current);
	}
	free(buf);
	if (r_addtolist == -1)
		return (-1);
	if (size_read == -1)
		return (0);
	return (1);
}

char	*extract_line(t_list_gnl *list, t_list_gnl **first)
{
	char	*result;
	int		i;
	int		j;
	int		eol;

	result = (char *)malloc(sizeof (char) * (get_size(*first) + 1));
	if (result == 0)
		return (gnl_lstclear(first, free), NULL);
	i = 0;
	while (list)
	{
		eol = list->eol;
		*first = list->next;
		j = 0;
		while (list->str[j])
			result[i++] = list->str[j++];
		free(list->str);
		free(list);
		if (eol)
			break ;
		list = *first;
	}
	result[i] = 0;
	return (result);
}

char	*get_next_line(int fd)
{
	static t_list_gnl	*list;
	char				*result;
	int					r_makelist;

	if (fd < 0)
		return (NULL);
	r_makelist = make_list(fd, &list);
	if (r_makelist == 0)
		return (NULL);
	if (r_makelist == -1)
	{
		gnl_lstclear(&list, free);
		return (NULL);
	}
	if (list == 0)
		return (NULL);
	result = extract_line(list, &list);
	return (result);
}
