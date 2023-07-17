/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jihalee <jihalee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 19:52:52 by jihalee           #+#    #+#             */
/*   Updated: 2023/07/07 16:49:39 by jihalee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# include <unistd.h>
# include <stdlib.h>
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

typedef struct s_list_gnl
{
	char				*str;
	int					eol;
	int					size;
	struct s_list_gnl	*next;
}				t_list_gnl;

char		*get_next_line(int fd);
char		*gnl_substr(char const *s, unsigned int start, size_t len);
void		find_nl(char *buf, int *i, int *j, int *eol_found);
int			add_to_list(char *buf, t_list_gnl **list, t_list_gnl **current);
int			make_list(int fd, t_list_gnl **list);
int			get_size(t_list_gnl *list);
char		*extract_line(t_list_gnl *list, t_list_gnl **first);
t_list_gnl	*gnl_lstlast(t_list_gnl *lst);
t_list_gnl	*gnl_lstnew(char *str, int size);
void		gnl_lstclear(t_list_gnl **lst, void (*del)(void *));

#endif
