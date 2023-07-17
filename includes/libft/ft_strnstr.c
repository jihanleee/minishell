/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jihalee <jihalee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 15:58:36 by jihalee           #+#    #+#             */
/*   Updated: 2023/05/08 13:54:09 by jihalee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i_big;
	size_t	i;

	if (big == 0 || little == 0)
		return (0);
	if (little[0] == 0)
		return ((char *)big);
	i_big = 0;
	while (big[i_big] && i_big < len)
	{
		if (big[i_big] == little[0])
		{
			i = 0;
			while (big[i_big + i] == little[i]
				&& little[i] && i_big + i < len)
				i++;
			if (little[i] == '\0')
				return (&(((char *)big)[i_big]));
		}
		i_big++;
	}
	return (0);
}
