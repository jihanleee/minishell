/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jihalee <jihalee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 21:33:15 by jihalee           #+#    #+#             */
/*   Updated: 2023/05/08 14:06:24 by jihalee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

char	*ft_strrchr(const char *s, int c)
{
	size_t	i;
	char	ch;

	ch = c;
	i = 0;
	while (s[i])
		i++;
	if (i == 0 && ch != 0)
		return (NULL);
	if (ch == '\0')
		return (&(((char *)s)[i]));
	i--;
	while (1)
	{
		if (s[i] == ch)
			return (&(((char *)s)[i]));
		if (i == 0)
			break ;
		i--;
	}
	return (0);
}
