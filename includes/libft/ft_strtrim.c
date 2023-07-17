/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jihalee <jihalee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 13:58:06 by jihalee           #+#    #+#             */
/*   Updated: 2023/05/08 14:02:23 by jihalee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static int	is_in_set(char c, char const *set)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

static void	copystr(char const *s1, char *result, int start, int end)
{
	int	i;
	int	j;

	i = start;
	j = 0;
	while (i <= end)
		result[j++] = s1[i++];
	result[j] = '\0';
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int		i;
	int		start;
	int		end;
	char	*result;

	if (!s1 || !set)
		return (0);
	i = 0;
	while (s1[i] != '\0' && is_in_set(s1[i], set))
		i++;
	start = i;
	while (s1[i] != '\0')
		i++;
	i--;
	while (is_in_set(s1[i], set) && i > start)
		i--;
	end = i;
	result = (char *)malloc(sizeof (char) * (end - start + 2));
	if (result == 0)
		return (NULL);
	copystr(s1, result, start, end);
	return (result);
}
