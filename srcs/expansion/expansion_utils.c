/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: solee2 <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 12:21:28 by solee2            #+#    #+#             */
/*   Updated: 2023/08/17 12:21:30 by solee2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*IFS characters are '\n', '\t' and ' ' by default in POSIX shell.*/
bool	is_ifs(char c)
{
	if (c == '\n' || c == '\t' || c == ' ')
		return (1);
	else
		return (0);
}

int	len_lvname(char *str)
{
	int	i;

	if (ft_isdigit(str[1]) || str[1] == '?')
		return (2);
	i = 1;
	while (ft_isalnum(str[i]) || str[i] == '_')
		i++;
	return (i);
}
