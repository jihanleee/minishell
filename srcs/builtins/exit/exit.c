/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: solee2 <solee2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 13:59:48 by solee2            #+#    #+#             */
/*   Updated: 2023/10/18 14:01:45 by solee2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_int(char *arg)
{
	int		i;

	i = 0;
	if (arg == NULL)
		return (1);
	if (arg[i] == '\0')
		return (1);
	if (arg[i] == '-' || arg[i] == '+')
		i++;
	while (arg[i])
	{
		if (arg[i] < '0' || arg[i] > '9')
			return (1);
		i++;
	}
	if (is_overflow(arg) == 1)
		return (1);
	return (0);
}

int	cnt_arg(t_job *current)
{
	int	cnt;

	cnt = 0;
	if (!current->arg)
		return (cnt);
	while (current->arg[cnt])
		cnt++;
	return (cnt);
}

void	free_exit(int exit_status, t_job *job, int fd)
{
	(void)fd;
	rl_clear_history();
	clear_env();
	clear_jobs(job);
	exit(exit_status);
}

void	exit_error_cases(int arg_cnt, t_job *current, int fd)
{
	if (arg_cnt > 1 && check_int(current->arg[0]) == 0)
	{
		write(2, "exit\n", 5);
		write(2, "bash: exit: too many arguments\n", 31);
		g_exit_stat = 1;
	}
	else
	{
		write(2, "exit\n", 5);
		write(2, "bash: exit: ", 12);
		write(2, current->arg[0], ft_strlen(current->arg[0]));
		write(2, ": numeric argument required\n", 28);
		g_exit_stat = 2;
		free_exit(2, current, fd);
	}
}

int	ft_exit(t_job **lst, int fd)
{
	t_job	*current;
	long	exit_num;
	int		arg_cnt;

	exit_num = 0;
	current = *lst;
	arg_cnt = cnt_arg(current);
	if (current->next)
		return (0);
	if (arg_cnt == 0)
		free_exit(g_exit_stat, current, fd);
	if (arg_cnt == 1 && check_int(current->arg[0]) == 0)
	{
		exit_num = ft_atol(current->arg[0]) % 256;
		g_exit_stat = exit_num;
		if (exit_num == 0)
			return (0);
		free_exit(exit_num, current, fd);
	}
	exit_error_cases(arg_cnt, current, fd);
	return (-1);
}
