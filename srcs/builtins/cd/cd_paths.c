/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_paths.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jihalee <jihalee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 13:59:00 by solee2            #+#    #+#             */
/*   Updated: 2023/08/17 22:26:36 by jihalee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cd_old_path(void)
{
	char	*new;

	new = find_param("OLDPWD");
	if (!new)
		cd_nonexist_error("\b\b");
	else if (access(new, F_OK) == -1)
		cd_nonexist_error(new);
	else if (is_dir(new) == FALSE)
		cd_dir_error(new);
	else if (access(new, X_OK) == 0)
	{
		change_update(new);
	}
	else
		cd_permission_error(new);
	free(new);
}

void	cd_normal_path(t_job *current)
{
	char	*path;

	path = getcwd(NULL, 0);
	if (path == NULL)
	{
		cd_dir_error(current->arg[0]);
		return ;
	}
	if (access(current->arg[0], F_OK) == -1)
		cd_nonexist_error(current->arg[0]);
	else if (is_dir(current->arg[0]) == FALSE)
		cd_dir_error(current->arg[0]);
	else if (access(current->arg[0], X_OK) == 0)
		change_update(current->arg[0]);
	else
		cd_permission_error(current->arg[0]);
	free(path);
}

void	cd_to_home(void)
{
	char	*new;

	new = find_param("HOME");
	if (!new)
		write(2, "bash: HOME: No such file or directory\n", 38);
	else if (access(new, F_OK) == -1)
		cd_nonexist_error(new);
	else if (is_dir(new) == FALSE)
		cd_dir_error(new);
	else if (access(new, X_OK) == 0)
		change_update(new);
	else
		cd_permission_error(new);
	free(new);
}

void	cd_absolute_path_two(t_job *current)
{
	if (access(current->arg[0], F_OK) == -1)
		cd_nonexist_error(current->arg[0]);
	else if (is_dir(current->arg[0]) == FALSE)
		cd_dir_error(current->arg[0]);
	else if (access(current->arg[0], X_OK) == 0)
		change_update(current->arg[0]);
	else
		cd_permission_error(current->arg[0]);
}

void	cd_absolute_path(t_job *current)
{
	char	*new;
	char	*path;

	if (!current->arg[0][1])
		cd_to_home();
	if (current->arg[0][0] == '~' && current->arg[0][1] == '/')
	{
		path = getenv("HOME");
		new = ft_strjoin(path, &current->arg[0][1]);
		if (access(new, F_OK) == -1)
			cd_nonexist_error(new);
		else if (is_dir(new) == FALSE)
			cd_dir_error(new);
		else if (access(new, X_OK) == 0)
			change_update(new);
		else
			cd_permission_error(new);
		free(new);
	}
}
