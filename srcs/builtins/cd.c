#include "minishell.h"



static char	*change_directory(char **env, char *new, int fd)
{
	int i;

	i = 0;
	while(new[i] && new[i] != '/')
	{

	}
	if (new[i])
		return (&new[i + 1]);
	return(NULL);
}

static int invalid_cd(char *path)
{
	int i;

	i = 0;
	printf("inside invalid cd\n");
	if (path[0] == '/' && path[1])	// '/' can only come in the first index if there's nothing afterwards
	{
		//printf("Something after /\n");
		return (1);
	}
	printf("before while loop\n");
	while (path[i])
	{
		if (i != 0 && i == '~')
			return (1);
		if (i != 0)
		{
			if (path[i - 1] == '/' && path[i] == '.' && path[i + 1] == '/')
				return (1);
		}
		i++;
	}
}


static void	cd_unset(char **env, char *arg)
{
	//t_job	*current;
	//int		length;
	int		i;

	//current = *lst;
	printf("\tinside cd_unset\n");
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], arg, get_length(arg)) == 0)
		{
			env[i] = NULL;
			free(env[i]);
			return ;
		}
		i++;
	}
}

static void	cd_export(char **env, char *arg, char *path)
{
	int length;
	int i;
	int j;
	int x;

	length = 0;	
	length += get_length(arg);
	length += get_length(path);
	printf("arg is %s\n", arg);
	printf("path is %s\n", path);
	printf("\tinside cd_export\n");
	i = 0;
	while(env[i])
		i++;
	printf("\tafter env loop\n");
	//env[i] = malloc(sizeof(char *) * (length + 1));
	env[i] = (char*)ft_calloc(length + 1, sizeof(char));
	if (!env[i])
		return ;
	j = 0;
	printf("\tafter malloc\n");
	while (j < get_length(arg))
	{
		env[i][j] = arg[j];
		j++;
	}
	x = 0;
	printf("\tafter arg loop\n");
	while (x < get_length(path))
	{
		env[i][j] = path[x];
		j++;
		x++;
	}
	printf("\tafter path loop\n");
}

int	ft_cd(t_job **lst, char **env, int fd)
{
	t_job	*current;
	char	*new;
	//int flag;
	int i;
	
	current = *lst;
	(void)fd;
	if (!current->arg || (current->arg[0][0] == '/' && !current->arg[0][1])) //not sure if sending to HOME is correct
	{
		//send to root
		new = getenv("HOME");
		printf("%s\n", new);
		cd_unset(env, "OLDPWD=");
		cd_export(env, "OLDPWD=", getenv("PWD"));
		cd_unset(env, "PWD=");
		cd_export(env, "PWD=", new);
		chdir(new);
		//free(new);
	}		
	else if (current->arg[0][0] == '-' && !current->arg[0][1])	//works on the first try but not for others..?
	{
		printf("received -\n");
		new = getenv("OLDPWD");
		printf("old path saved as %s\n", new);
		cd_unset(env, "PWD=");
		cd_export(env, "PWD=", getenv("OLDPWD"));
		cd_unset(env, "OLDPWD=");
		cd_export(env, "OLDPWD=", getcwd(NULL,0));
		chdir(new);

		/*
		new = getenv("PWD");
		printf("%s\n", new);
		cd_unset(env, "PWD=");
		cd_export(env, "PWD=", getenv("OLDPWD"));
		chdir(getenv("OLDPWD"));
		cd_unset(env, "OLDPWD=");
		cd_export(env, "OLDPWD=", getcwd(NULL, 0));
		*/
		/*
		cd_unset(env, "PWD=");
		cd_export(env, "PWD=", getenv("OLDPWD"));
		chdir(getenv("PWD"));
		cd_unset(env, "OLDPWD=");
		cd_export(env, "OLDPWD=", getenv("PWD"));
		*/
	}
	else if (current->arg[0][0] == '~' && (!current->arg[0][1] || current->arg[0][1] == '/'))	//works
	{
		printf("received ~\n");
		new = getenv("HOME");
		printf("%s\n", new);
		cd_unset(env, "OLDPWD=");
		cd_export(env, "OLDPWD=", getenv("PWD"));
		cd_unset(env, "PWD=");
		cd_export(env, "PWD=", new);
		chdir(new);
	}
	else	//seems to work..?
	{
		if (invalid_cd(current->arg[0]) == 1)
		{
			printf("No Such Directory\n");
			return (1);
		}
		//cd_reset(env, "OLDPWD=", getenv("PWD"));
		cd_unset(env, "OLDPWD=");
		cd_export(env, "OLDPWD=", getenv("PWD"));
		chdir(current->arg[0]);
		cd_unset(env, "PWD=");
		cd_export(env, "PWD=", getcwd(NULL, 0));
		//cd_reset(env, "PWD=", current->arg[0]);
	}
	return (0);
}