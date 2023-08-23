/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: solee2 <solee2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 14:00:25 by solee2            #+#    #+#             */
/*   Updated: 2023/08/17 14:46:03 by solee2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pwd(int fd)
{
	char	*print;

	print = getcwd(NULL, 0);
	if (!print)
	{
		ft_putstr_fd("pwd: error retrieving current directory: ", 2);
		ft_putstr_fd("getcwd: cannot access: ", 2);
		ft_putstr_fd("No such file or directory\n", 2);
	}
	else
	{
		ft_putstr_fd(print, fd);
		write(fd, "\n", 1);
		free(print);
	}
	return (0);
}
