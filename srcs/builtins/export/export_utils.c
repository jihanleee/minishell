/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jihalee <jihalee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 14:00:15 by solee2            #+#    #+#             */
/*   Updated: 2023/08/23 16:34:08 by jihalee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	first_is_valid(char c)
{
	if (c == '=')
		return (1);
	if (ft_isalpha(c) == 0 && c != '_')
		return (1);
	return (0);
}

int	middle_is_valid(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\\')
			i++;
		if (str[i] == '=' && i == 0)
			return (1);
		if (str[i] == '=' && i != 0)
			break ;
		if (ft_isalnum(str[i]) == 0 && str[i] != '_')
			return (1);
		i++;
	}
	return (0);
}

int	middle_error_case(char *str)
{
	int	i;
	int	flag;

	i = 1;
	flag = 0;
	while (str[i])
	{
		if (str[i] == '\\')
			i++;
		if (str[i] == '=' && i == 0)
			return (1);
		if (str[i] == '=' && i != 0)
		{	
			flag = 1;
			break ;
		}
		if (ft_isalnum(str[i]) == 0 && str[i] != '_')
			return (1);
		i++;
	}
	if (flag == 0)
		return (1);
	return (0);
}

void	export_error(char *str)
{
	write(2, "bash: export: \'", 15);
	write(2, str, ft_strlen(str));
	write(2, "\': not a valid identifier\n", 26);
	g_exit_stat = 1;
}

void	export_env(int fd)
{
	t_env	**env;
	t_env	*current;
	int		i;

	env = get_env_address();
	current = *env;
	if (!current)
		return ;
	while (current)
	{
		ft_putstr_fd("export ", fd);
		if (*((ft_strchr(current->str, '=') + 1)))
			ft_putstr_fd(current->str, fd);
		else
		{
			i = 0;
			while (current->str[i] && current->str[i] != '=')
				ft_putchar_fd(current->str[i++], fd);
		}
		ft_putstr_fd("\n", fd);
		current = current->next;
	}
}
