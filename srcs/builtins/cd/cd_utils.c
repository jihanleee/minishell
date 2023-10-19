/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: solee2 <solee2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 13:59:08 by solee2            #+#    #+#             */
/*   Updated: 2023/10/19 11:14:38 by solee2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cd_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while ((s1[i] || s2[i]) && i < n)
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}

void	cd_unset(char *arg)
{
	t_env	**env;
	t_env	*current;
	t_env	*next;

	env = get_env_address();
	current = (*env);
	if (((*env) && (*env)->str) && \
		(cd_strncmp(current->str, arg, ft_strlen(arg)) == 0))
	{
		next = current->next;
		free((free(current->str), current));
		(*env) = next;
	}
	current = (*env);
	while (current)
	{
		next = current->next;
		if (next && cd_strncmp(next->str, arg, ft_strlen(arg)) == 0)
		{
			current->next = current->next->next;
			free((free(next->str), next));
			break ;
		}
		current = current->next;
	}
}

bool	is_dir(char *path)
{
	DIR	*dir;

	dir = opendir(path);
	if (dir)
	{
		closedir(dir);
		return (TRUE);
	}
	return (FALSE);
}

void	cd_to_root(void)
{
	char	*cwd;
	char	*line;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		cwd = find_param("PWD");
	if (cwd)
	{
		cd_unset("OLDPWD");
		line = ft_strjoin("OLDPWD=", cwd);
		free(cwd);
		add_to_env(line);
		free(line);
	}
	chdir("/");
	g_exit_stat = 0;
	cd_unset("PWD");
	add_to_env("PWD=/");
}
