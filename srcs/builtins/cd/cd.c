/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jihalee <jihalee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 13:58:47 by solee2            #+#    #+#             */
/*   Updated: 2023/08/23 17:19:31 by jihalee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	cd_export(char *str)
{
	t_env	**env;
	t_env	*current;

	env = get_env_address();
	if (*env == NULL)
	{
		*env = (t_env *)ft_calloc(1, sizeof(t_env));
		(*env)->str = ft_strdup(str);
	}
	else
	{
		current = (*env);
		while (current && current->next != NULL)
			current = current->next;
		current->next = (t_env *)ft_calloc(1, sizeof (t_env));
		current->next->str = ft_strdup(str);
	}
	return (0);
}

void	change_update(char *new)
{
	char	*cwd_one;
	char	*cwd_two;
	char	*line_one;
	char	*line_two;

	cwd_one = getcwd(NULL, 0);
	if (!cwd_one)
		cwd_one = find_param("PWD");
	cd_unset("OLDPWD");
	if (cwd_one)
	{
		line_one = ft_strjoin("OLDPWD=", cwd_one);
		cd_export(line_one);
		free((free(cwd_one), line_one));
	}
	chdir(new);
	g_exit_stat = 0;
	cwd_two = getcwd(NULL, 0);
	cd_unset("PWD");
	line_two = ft_strjoin("PWD=", cwd_two);
	cd_export(line_two);
	free((free(cwd_two), line_two));
}

void	toomanyargs(void)
{
	g_exit_stat = 1;
	write(2, "bash: cd: too many arguments\n", 29);
}

void	ft_cd(t_job **lst)
{
	t_job	*current;
	char	*cwd;

	current = *lst;
	cwd = getcwd(NULL, 0);
	if (!current->arg)
		cd_to_home();
	else if (current->arg && current->arg[1])
		toomanyargs();
	else if (current->arg[0][0] == '/' && !current->arg[0][1])
		cd_to_root();
	else if (current->arg[0][0] == '-' && !current->arg[0][1])
		cd_old_path();
	else if (current->arg[0][0] == '~' && \
		(!current->arg[0][1] || current->arg[0][1] == '/'))
		cd_absolute_path(current);
	else if (ft_strchr(current->arg[0], '/'))
		cd_absolute_path_two(current);
	else if (invalid_cd(current->arg[0]) == 1)
		cd_nonexist_error(current->arg[0]);
	else
		cd_normal_path(current);
	free(cwd);
}
