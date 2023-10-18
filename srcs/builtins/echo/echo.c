/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: solee2 <solee2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 13:59:25 by solee2            #+#    #+#             */
/*   Updated: 2023/10/18 14:57:01 by solee2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	print_args(t_job *current, int i, int fd)
{
	int	j;

	while (current->arg[i])
	{
		j = 0;
		while (current->arg[i][j])
		{
			if (current->arg[i][j])
				write(fd, &current->arg[i][j], 1);
			j++;
		}
		i++;
		if (current->arg[i])
			write(fd, " ", 1);
	}
	return (0);
}

void	ft_echo(t_job *current, int fd)
{
	int		i;
	int		j;
	int		flag;

	flag = 0;
	i = 0;
	if (current->arg)
	{	
		while (current->arg[i] && ft_strncmp(current->arg[i], "-n", 2) == 0)
		{
			j = 2;
			while (current->arg[i][j] == 'n')
				j++;
			if (current->arg[i][j] == 0)
			{
				i++;
				flag = 1;
			}
			else
				break ;
		}
		print_args(current, i, fd);
	}
	if (flag == 0)
		write(fd, "\n", 1);
}
