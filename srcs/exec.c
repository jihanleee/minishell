
#include "minishell.h"

int	g_status = 0;
/*
int ft_strlen(char *s)
{
    int len = 0;
    if (!s)
        return (0);
    while (*s)
    {
        len++;
        s++;
    }
    return (len);
}

char	*ft_strdup(char *src)
{
	char	*dest;
	int		len;
	int 	i;

	if (!src)
		return (0);
	len = ft_strlen(src);
	dest = (char *)malloc(sizeof(char) * (len + 1));
	if (!dest)
		return (0);
	i = 0;
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char    *ft_strjoin(char *s1, char *s2)
{
    char    *str;
    int     s1_len;
    int     s2_len;
    int     i;
    int     j;

    i = -1;
    j = -1;
    if (s1 == 0 || s2 == 0)
        return (0);
    s1_len = ft_strlen(s1);
    s2_len = ft_strlen(s2);
    str = (char *)malloc(sizeof(char) * (s1_len + s2_len + 1));
    if (!str)
        return (0);
    while (++i < s1_len)
        str[i] = s1[i];
    while (++j < s2_len)
        str[i++] = s2[j];
    str[i] = 0;
    return (str);
}

int ft_lexeme_count(char const *s, char c)
{
    int i;
    int cnt;

    i = 0;
    cnt = 0;
    while (s[i])
    {
        if (s[i] == c)
            i++;
        else
        {
            cnt++;
            while (s[i] && s[i] != c)
                i++;
        }
    }
    return (cnt);
}

char    *ft_lexeme_make(char *lexeme, char const *s, int k, int lexeme_len)
{
    int     i;

    i = 0;
    while (lexeme_len > 0)
        lexeme[i++] = s[k - lexeme_len--];
    lexeme[i] = '\0';
    return (lexeme);
}

char    **ft_split2(char **result, char const *s, char c, int lexeme_num)
{
    int     i;
    int     k;
    int     lexeme_len;

    i = 0;
    k = 0;
    lexeme_len = 0;
    while (s[k] && i < lexeme_num)
    {
        while (s[k] && s[k] == c)
            k++;
        while (s[k] && s[k] != c)
        {
            k++;
            lexeme_len++;
        }
        result[i] = (char *)malloc(sizeof(char) * (lexeme_len + 1));
        if (!(result[i]))
            return (NULL);
        ft_lexeme_make(result[i], s, k, lexeme_len);
        lexeme_len = 0;
        i++;
    }
    result[i] = 0;
    return (result);
}

char    **ft_split(char const *s, char c)
{
    int     lexeme_num;
    char    **result;

    if (s == 0)
        return (NULL);
    lexeme_num = ft_lexeme_count(s, c);
    result = (char **)malloc(sizeof(char *) * (lexeme_num + 1));
    if (!(result))
        return (NULL);
    ft_split2(result, s, c, lexeme_num);
    return (result);
}

int ft_strncmp(const char *s1, const char *s2, size_t n)
{
    unsigned int    i;

    i = 0;
    if (n == 0 || !s1)
        return (0);
    while (s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0' && i < n - 1)
        i++;
    return (s1[i] - s2[i]);
}
*/

char    *ft_cmd(char **path, char *cmd)
{
    int     i;
    char    *path_cmd;
    char    *temp;

    if (access(cmd, X_OK) == 0)
        return (cmd);
    i = -1;
    while (path[++i])
    {
        temp = ft_strjoin(path[i], "/");
        path_cmd = ft_strjoin(temp, cmd);
        free(temp);
        if (access(path_cmd, X_OK) == 0)
            return (path_cmd);
        free(path_cmd);
    }
    return (NULL);
}

char    **trim_path(char **env)
{
    int     i;
    char    **path;

    i = -1;
    while (env[++i])
    {
        if (ft_strncmp("PATH", env[i], 4) == 0)
            break ;
    }
    path = ft_split(env[i] + 5, ':');
    return (path);
}

char *get_full_path(char **env, char *cmd)
{
	char **path;
	char *full_path;

	printf("in get full cmd function\n");
	path = trim_path(env);
	full_path = ft_cmd(path, cmd);
	if (!full_path)
	{
		while (*path)
			free(*path++);
		free(path);
		perror("Command error");
		exit(1);
	}
	return (full_path);
}

int	count_cmd(char **cmd)
{
	int 	cnt;
	char	**arg;

	arg = cmd;
	cnt = 0;
	while (arg[cnt])
		cnt++;
	return (cnt);
}

char	**get_full_cmd(t_pipe *cmd_line)
{
	char **full;
	int		len;
	int		i;

	//printf("in get full cmd function\n");
	i = 0;
	len = 0; //
	if (cmd_line->arg)
		len = count_cmd(cmd_line->arg);
	full = (char **)malloc(sizeof(char *) * (len + 2));
	if(!full)
		return (NULL);
	//printf("get full cmd here\n");
	full[0] = ft_strdup(cmd_line->cmd);
	while(i < len)
	{
		full[i + 1] = ft_strdup(cmd_line->arg[i]);
		i++;
	}
	full[i + 1] = NULL;
	//printf("get full cmd ending\n");
	return (full);
}

int	non_builtin(t_pipe *cmd_line, char **env, int fds[])
{
	char	*full_path;
	char	**full_cmd;
	int		status;
	pid_t	pid;

	full_cmd = get_full_cmd(cmd_line);
	full_path = get_full_path(env, cmd_line->cmd);
	cmd_line->cmd = full_path;
	pid = fork();
	if (pid < 0)
		return (-1);
	if (pid == 0)
	{
		if (cmd_line->next && cmd_line->out == 0)
			dup2(fds[1], 1);
		if (execve(cmd_line->cmd, full_cmd, env) == -1)
			return (-1);
	}
	else
	{
		waitpid(pid, &status, 0);
		g_status = status >> 8;
	}
	return (0);
}
int	redirect_fd(int old_fd, int new_fd)
{
	if (old_fd != new_fd)
	{
		if (dup2(old_fd, new_fd) == -1)
		{
			perror("Error duplicating file descriptor");
			exit(1);
		}
		close(old_fd);
	}
	return (new_fd);
}

void right_redir(t_pipe *cmd_line)
{
	if (cmd_line->out == 3 && cmd_line->outfile) 
	{
		int flags = O_WRONLY | O_CREAT | O_TRUNC;
		int output_fd = open(cmd_line->outfile, flags, 0744);
		if (output_fd < 0)
		{
			perror("Error opening output file");
			exit(1);
		}
		redirect_fd(output_fd, STDOUT_FILENO);
	}
	else if (cmd_line->out == 4 && cmd_line->outfile) 
	{
		int flags = O_WRONLY | O_CREAT | O_APPEND;
		int output_fd = open(cmd_line->outfile, flags, 0744);
		if (output_fd < 0) 
		{
			perror("Error opening output file");
			exit(1);
		}
		redirect_fd(output_fd, STDOUT_FILENO);
	}
}

void left_redir(t_pipe *cmd_line)
{
	if (cmd_line->in == 1 && cmd_line->infile) 
	{
		int input_fd = open(cmd_line->infile, O_RDONLY);
		if (input_fd < 0) 
		{
			perror("Error opening input file");
			exit(1);
		}
		redirect_fd(input_fd, STDIN_FILENO);
	}
}

void	apply_redir(t_pipe *cmd_line)
{
	left_redir(cmd_line);
	right_redir(cmd_line);
}

void	ft_putstr_fd(char const *s, int fd)
{
	if (s == 0)
		return ;
	while (*s)
		write(fd, s++, 1);
}

/*
int ft_pwd(int fd)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	ft_putstr_fd(pwd, fd);
	write(fd, "\n", 1);
	free(pwd);
	return (1);
}
*/

int exec_function(t_pipe *cmd_line, char ***env, int end[])
{
	int fd;
	//printf("inside exec function\n");
	//printf("current command is %s\n", cmd_line->cmd);
	/*
	if (cmd_line->arg[0])
		printf("command's arg is %s\n", cmd_line->arg[0]);	
	printf("after printing arg\n");
	*/
	if (cmd_line == NULL)
		return (-1);
	if (cmd_line->in != 0 || cmd_line->out != 0)
		apply_redir(cmd_line);
	if (cmd_line->next && cmd_line->in == 0 && cmd_line->out == 0)
		fd = end[1];
	if (!cmd_line->next)
		fd = STDOUT_FILENO;
	//printf("recognized builtin\n");
	if (ft_strncmp("pwd", cmd_line->cmd, 4) == 0)
		return (ft_pwd(&cmd_line, *env, fd));
    else if (ft_strncmp("cd", cmd_line->cmd, 3) == 0)
        return (ft_cd(&cmd_line, *env, fd));
    else if (ft_strncmp("exit", cmd_line->cmd, 5) == 0)
        return (ft_exit(&cmd_line, *env, fd));
    else if (ft_strncmp("env", cmd_line->cmd, 4) == 0)
        //return (ft_env(&cmd_line, *env, fd));
		ft_env(&cmd_line, *env, fd);
	else if (ft_strncmp("export", cmd_line->cmd, 7) == 0)
        return (ft_export(&cmd_line, *env, fd));
    else if (ft_strncmp("echo", cmd_line->cmd, 5) == 0)
        //return (ft_echo(&cmd_line, *env, fd));
		ft_echo(&cmd_line, *env, fd);
	else if (ft_strncmp("unset", cmd_line->cmd, 6) == 0)
        return (ft_unset(&cmd_line, *env, fd));
	else if (!non_builtin(cmd_line, *env, end)) 
		return (-1);
	return (0);
}

int	exec(t_pipe *cmd_line, char **env)
{
	int		end[2];
	int		status;
	pid_t	pid;
	int		tmp;
	int		res;

	if (cmd_line == NULL)
		return (g_status);
	pipe(end);
	exec_function(cmd_line, &env, end);
	res = exec_function(cmd_line, &env, end);
	if (res == -1)
		return (-1);
	else
		g_status = 0;
	if (cmd_line->next)
	{
		pid = fork();
		if (pid < 0)
		{
			perror("Error forking process");
			return (g_status);
		}
	}
	else
		return (g_status);
	if (pid == 0)
	{
		dup2(end[0], 0);
		close(end[1]);
		g_status = exec(cmd_line->next, env);
		exit(g_status);
	}
	else
	{
		close(end[1]);
		close(end[0]);
		waitpid(pid, &status, 0);
		g_status = status >> 8;
		return (g_status);
	}
}


void exec_command_line(t_pipe *temp, char **env)
{
	//printf("inside exec commnad line\n");
	//printf("\tcommand: %s\n", temp->cmd);
	//printf("\targ: %s\n", temp->arg[0]);
	exec(temp, env);
}

/*
t_pipe *new_cmd(char *cmd, char **arg, int in, int out, char *infile, char *outfile) 
{
	t_pipe *cmd_line = (t_pipe *)malloc(sizeof(t_pipe));
	if (cmd_line == NULL)
	{
		perror("Error allocating memory for t_pipe");
		exit(1);
	}
	cmd_line->cmd = cmd;
	cmd_line->arg = arg;
	cmd_line->in = in;
	cmd_line->out = out;
	cmd_line->infile = infile;
	cmd_line->outfile = outfile;
	cmd_line->next = NULL;
	return (cmd_line);
}


int main(int ac, char **av, char **env)
{
	(void)ac;
	(void)av;

	char *first_opt[] = {"-l", NULL};
	t_pipe *first_cmd = new_cmd("ls", first_opt, 0, 0, NULL, NULL);

	char *opt1[] = {".c", NULL};
	t_pipe *second_cmd = new_cmd("grep",opt1, 0, 0, NULL, NULL);

	char *opt2[] = {"-l", NULL};
	t_pipe *third_cmd = new_cmd("wc",opt2, 0, 3, NULL, "outfile");

	first_cmd->next = second_cmd;
	second_cmd->next = third_cmd;
	third_cmd->next = NULL;

	exec_command_line(first_cmd, env);

	t_pipe *temp;
	while (first_cmd)
	{
		temp = first_cmd;
		first_cmd = first_cmd->next;
		free(temp);
	}
    return (0);
}
*/