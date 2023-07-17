/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jihalee <jihalee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/29 17:40:03 by jihalee           #+#    #+#             */
/*   Updated: 2023/05/08 13:50:00 by jihalee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*result;
	size_t	total;

	total = size * nmemb;
	if (size != 0 && nmemb != 0 && (total < nmemb || total < size))
		return (NULL);
	result = (void *)malloc(total);
	if (result == 0)
		return (0);
	ft_bzero(result, total);
	return (result);
}
