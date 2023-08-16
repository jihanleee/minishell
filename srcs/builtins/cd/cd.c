#include "minishell.h"

void	change_update(char *new)
{
	char	*cwd_one;
	char	*cwd_two;
	char	*line_one;
	char	*line_two;

	cwd_one = getcwd(NULL, 0);
	cd_unset("OLDPWD");
	line_one = ft_strjoin("OLDPWD=", cwd_one);
	add_to_env(line_one);
	free((free(cwd_one), line_one));
	chdir(new);
	cwd_two = getcwd(NULL, 0);
	cd_unset("PWD");
	line_two = ft_strjoin("PWD=", cwd_two);
	add_to_env(line_two);
	free((free(cwd_two), line_two));
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
		write(2, "bash: cd: too many arguments\n", 29);
	else if (path_compare(current->arg[0], cwd) == 0)
	{
		return (free(cwd));
	}
	else if (current->arg[0][0] == '/' && !current->arg[0][1])
		cd_to_root();
	else if (current->arg[0][0] == '-' && !current->arg[0][1])
		cd_old_path();
	else if (current->arg[0][0] == '~' && \
		(!current->arg[0][1] || current->arg[0][1] == '/'))
		cd_absolute_path(current);
	else if (invalid_cd(current->arg[0]) == 1)
		cd_error(current->arg[0]);
	else
		cd_normal_path(current);
	free(cwd);
}
