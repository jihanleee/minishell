/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jihalee <jihalee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 15:30:32 by jihalee           #+#    #+#             */
/*   Updated: 2023/07/17 17:00:12 by jihalee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_valid_base(char *base)
{
	int	start_index;
	int	index;

	if (base[0] == '\0' || base[1] == '\0')
		return (0);
	start_index = 0;
	while (base[start_index])
	{
		index = start_index + 1;
		while (base[index])
		{
			if (base[start_index] == base[index])
				return (0);
			index++;
		}
		if (base[start_index] == '+'
			|| base[start_index] == '-'
			|| (base[start_index] <= 32 || base[start_index] > 126))
			return (0);
		start_index++;
	}
	return (1);
}

static int	ft_ctoi(char c, char *base)
{
	int	index;

	index = 0;
	while (base[index])
	{
		if (base[index] == c)
			return (index);
		index++;
	}
	return (-1);
}

int	ft_atoi_base(char *str, char *base)
{
	int	index;
	int	sign;
	int	place_val;
	int	result;

	if (is_valid_base(base) == 0)
		return (0);
	index = 0;
	while (str[index] == ' ' || (str[index] >= 9 && str[index] <= 13))
		index++;
	sign = 1;
	while (str[index] == '+' || str[index] == '-')
		if (str[index++] == '-')
			sign *= -1;
	while (ft_ctoi(str[index], base) != -1)
		index++;
	index--;
	result = 0;
	place_val = 1;
	while (ft_ctoi(str[index], base) != -1)
	{
		result += ft_ctoi(str[index--], base) * place_val;
		place_val *= ft_strlen(base);
	}
	return (result * sign);
}
