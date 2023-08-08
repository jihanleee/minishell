
#include "minishell.h"

char	*ft_job(char **path, char *cmd)
{
	int		i;
	char	*path_cmd;
	char	*temp;

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

char	**trim_path(char **env)
{
	int		i;
	char	**path;

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
	char	**path;
	char	*full_path;

	// printf("in get full cmd function\n");
	path = trim_path(env);
	full_path = ft_job(path, cmd);
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

int	count_job(char **cmd)
{
	int		cnt;
	char	**arg;

	arg = cmd;
	cnt = 0;
	while (arg[cnt])
		cnt++;
	return (cnt);
}

char	**get_full_cmd(t_job *cmd_line)
{
	char	**full;
	int		len;
	int		i;

	i = 0;
	len = 0;
	if (cmd_line->arg)
		len = count_job(cmd_line->arg);
	full = (char **)malloc(sizeof(char *) * (len + 2));
	if (!full)
		return (NULL);
	//printf("get full cmd here\n");
	full[0] = ft_strdup(cmd_line->cmd);
	while(i < len)
	{
		full[i + 1] = ft_strdup(cmd_line->arg[i]);
		i++;
	}
	full[i + 1] = NULL;
	//printf("get full cmd pipefding\n");
	return (full);
}

void		exec_child_process(t_job *cmd_line, char **env)
{
	int		ret;
	char	*full_path;
	char	**full_cmd;

	ret = EXIT_SUCCESS;
	full_cmd = get_full_cmd(cmd_line);
	full_path = get_full_path(env, cmd_line->cmd);
	cmd_line->cmd = full_path;
	if (cmd_line->next)
	{
		close(cmd_line->next->pipefd[0]);
		dup2(cmd_line->next->pipefd[1], STDOUT_FILENO);
		close(cmd_line->next->pipefd[1]);
	}
	if (cmd_line->pipefd[0] != 0)
	{
		close(cmd_line->pipefd[1]);
		dup2(cmd_line->pipefd[0], STDIN_FILENO);
		close(cmd_line->pipefd[0]);
	}
	// if (check_builtin(cmd->cmdlines) == 1)
	// 	exec_builtin(cmd, cmd->cmdlines);
	// else
		ret = execve(full_path, full_cmd, env);
	if (ret == -1)
		perror("error execve");
	exit(ret);
}

int exec_cmd(t_job *cmd_line, char **env)
{
	pid_t pid;
	int ret = EXIT_SUCCESS;

	if (cmd_line->next)
		pipe(cmd_line->next->pipefd);
	pid = fork();
	if (pid < 0)
	{
		perror("Error forking process");
		return (ret);
	}
	if (pid == 0)
	{
		exec_child_process(cmd_line, env);
	}
	if (cmd_line->next)
		close(cmd_line->next->pipefd[1]);
	if (cmd_line->pipefd[0] != 0)
		close(cmd_line->pipefd[0]);
	return (ret);
}

void close_remaining_fds(t_job *cmd_line)
{
	t_job *curr_job = cmd_line;

	while (curr_job != NULL)
	{
		if (curr_job->next)
			close(curr_job->next->pipefd[1]); // Close write end of pipe
		if (curr_job->pipefd[0] != 0)
			close(curr_job->pipefd[0]); // Close read end of pipe

		curr_job = curr_job->next;
	}
}

int	exec_builtin(t_job *cmd_line, char **env, int fd)
{
	if (ft_strncmp("pwd", cmd_line->cmd, 4) == 0)
		ft_pwd(&cmd_line, env, fd);
	else if (ft_strncmp("cd", cmd_line->cmd, 3) == 0)
		ft_cd(&cmd_line, env, fd);
	else if (ft_strncmp("echo", cmd_line->cmd, 5) == 0)
		ft_echo(&cmd_line, env, fd);
	else if (ft_strncmp("env", cmd_line->cmd, 4) == 0)
		ft_env(&cmd_line, env, fd);
	else if (ft_strncmp("export", cmd_line->cmd, 7) == 0)
		ft_export(&cmd_line, env, fd);
	else if (ft_strncmp("unset", cmd_line->cmd, 6) == 0)
		ft_unset(&cmd_line, env, fd);
	else if (ft_strncmp("exit", cmd_line->cmd, 5) == 0)
		ft_exit(&cmd_line, env, fd);
	else
		return (0);
	return (1);
}

int check_builtin(char *cmd)
{
	if (!ft_strncmp(cmd, "cd", 3) || !ft_strncmp(cmd, "echo", 5)
		|| !ft_strncmp(cmd, "pwd", 4) || !ft_strncmp(cmd, "env", 4)
		|| !ft_strncmp(cmd, "export", 7) || !ft_strncmp(cmd, "export", 7)
		|| !ft_strncmp(cmd, "unset", 6) || !ft_strncmp(cmd, "exit", 5))
	{
		return (1);
	}
	return (0);
}


void exec_command_line(t_job *cmd_line, char **env)
{
	t_job *curr_job;
	int status;

	curr_job = cmd_line;
	while (curr_job != NULL)
	{
		// if (cmd_line->cmd)
		// {
		// 	if (cmd_line->in != 0 || cmd_line->out != 0)
		// 		apply_redir(cmd_line);
		// }
		if ((check_builtin(cmd_line->cmd) == 1)) //여기서 그냥 처리해버릴까...? 아님 자식 프로세스 안에서?
			exec_builtin(cmd_line, env, STDOUT_FILENO);
		else
			exec_cmd(curr_job, env);
        curr_job = curr_job->next;
    }

    // wait for all child proce to complete.......jebal..
	while (wait(&status) > 0)
	{

	}
    close_remaining_fds(cmd_line);
}

/*
int parse_cmd(t_job **cmd, char **av)
{
    t_job *new;
    int cnt_word;

    if (!(new = (t_job *)malloc(sizeof(t_job))))
        return (0);
    int i = 0;
    while (av[i] && strcmp(av[i], "|") != 0)
        i++;
    if (!(new->av = (char **)malloc(sizeof(char *) * (i + 1))))
        return (0);
    new->cnt_word= i;
    new->av[i] = NULL;
    while (--i >= 0)
        new->av[i] = strdup(av[i]);
    new->prev = NULL;
    new->next = NULL;
    ft_lst_add_back(cmd, new);
    return (new->cnt_word);
}

int main(int ac, char **av, char **env)
{
    t_job *cmd;
    int i;

    if (ac <= 1)
        return (0); // 에러 메세지 출력하기
    i = 1;
    cmd = NULL;
    while (av[i])
    {
        i += parse_cmd(&cmd, &av[i]); // 파싱해서 cmd 에 저장
        if (!av[i]) // 다음 인자 없으면 여기서 파싱 끝
            break;
        else
            i++;
    }
    if (cmd)
        ft_execute(cmd, env);
    //ft_free(cmd); 나중에
    return (0);
}
*/