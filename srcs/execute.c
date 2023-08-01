
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

typedef struct s_cmd
{
    char    **av;               
    int     end_type;
    int     end[2];
    char    *redirect_input;
    char    *redirect_output;
    int     append_flag;
    struct  s_cmd *prev;
    struct  s_cmd *next;
} t_cmd;

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

char	*ft_strjoin(char *s1, char *s2)
{
	char	*str;
	int		s1_len;
	int		s2_len;
	int		i;
	int		j;

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

int	ft_lexeme_count(char const *s, char c)
{
	int	i;
	int	cnt;

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

char	*ft_lexeme_make(char *lexeme, char const *s, int k, int lexeme_len)
{
	int		i;

	i = 0;
	while (lexeme_len > 0)
		lexeme[i++] = s[k - lexeme_len--];
	lexeme[i] = '\0';
	return (lexeme);
}

char	**ft_split2(char **result, char const *s, char c, int lexeme_num)
{
	int		i;
	int		k;
	int		lexeme_len;

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

char	**ft_split(char const *s, char c)
{
	int		lexeme_num;
	char	**result;

	if (s == 0)
		return (NULL);
	lexeme_num = ft_lexeme_count(s, c);
	result = (char **)malloc(sizeof(char *) * (lexeme_num + 1));
	if (!(result))
		return (NULL);
	ft_split2(result, s, c, lexeme_num);
	return (result);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned int	i;

	i = 0;
	if (n == 0 || !s1)
		return (0);
	while (s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0' && i < n - 1)
		i++;
	return (s1[i] - s2[i]);
}

char	*ft_cmd(char **path, char *cmd)
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

char	**trim_path(char **envp)
{
    int		i;
    char	**path;

    i = -1;
    while (envp[++i])
    {
        if (ft_strncmp("PATH", envp[i], 4) == 0)
            break ;
    }
    path = ft_split(envp[i] + 5, ':');
    return (path);
}

char *get_full_path(char **env, char *cmd)
{
    char **path = trim_path(env);
    char *full_path = ft_cmd(path, cmd);

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

int redirect_fd(int old_fd, int new_fd) 
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
    return new_fd;
}

void apply_redirection(t_cmd *cmd) 
{
    if (cmd->redirect_input) 
    {
        int input_fd = open(cmd->redirect_input, O_RDONLY);
        if (input_fd < 0) 
        {
            perror("Error opening input file");
            exit(1);
        }
        redirect_fd(input_fd, STDIN_FILENO);
    }

    if (cmd->redirect_output) 
    {
        int flags = O_WRONLY | O_CREAT;
        if (cmd->append_flag) 
            flags |= O_APPEND;
        else 
            flags |= O_TRUNC;
        int output_fd = open(cmd->redirect_output, flags, 0644);
        if (output_fd < 0) 
        {
            perror("Error opening output file");
            exit(1);
        }
        redirect_fd(output_fd, STDOUT_FILENO);
    }
}

void    apply_pipe(t_cmd *cmd)
{
    if (cmd->end_type == 1) 
    {
        // 파이프를 사용하는 경우 파이프로 리다이렉션
        redirect_fd(cmd->end[0], STDIN_FILENO);
        close(cmd->end[1]);
    }

    if (cmd->prev && cmd->prev->end_type == 1) 
    {
        // 이전 명령어가 파이프를 사용하는 경우 파이프로 리다이렉션
        redirect_fd(cmd->prev->end[1], STDOUT_FILENO);
        close(cmd->prev->end[0]);
    }
}


void handle_child_process(t_cmd *cmd, char **env, int in_fd) 
{
    char *full_path;

    apply_redirection(cmd);
    apply_pipe(cmd);
    if (in_fd != STDIN_FILENO) 
	{
        if (dup2(in_fd, STDIN_FILENO) < 0) {
            perror("Error duplicating file descriptor");
            exit(1);
        }
    }
    full_path = get_full_path(env, cmd->av[0]);
    cmd->av[0] = full_path;
    if (execve(cmd->av[0], cmd->av, env) < 0) {
        perror("Error executing command");
        exit(1);
    }
}

void	handle_parent_process(t_cmd *cmd, int pid, int has_pipe)
{
	int	status;

	waitpid(pid, &status, 0);
	if (has_pipe)
	{
		close(cmd->end[1]);
		if (!cmd->next || cmd->end_type != 1)
			close(cmd->prev->end[0]);
		if (cmd->prev && cmd->prev->end_type == 1)
			close(cmd->prev->end[0]);
	}
}

void	ft_putstr_fd(char const *s, int fd)
{
	if (s == 0)
		return ;
	while (*s)
		write(fd, s++, 1);
}


int	ft_pwd(int fd)
{
	char		*str;

	str = getcwd(NULL, 0);
	ft_putstr_fd(str, fd);
	write(fd, "\n", 1);
	free(str);
	return (1);
}

int execute_builtin_with_pipe(t_cmd *cmd, int (*builtin_func)(int), int fd) 
{
    int pipefd[2];

    if (cmd->end_type == 1) 
	{ // 파이프 뒤에 명령어가 있을 경우
        pipe(pipefd);
        fd = pipefd[1]; // 파이프에 쓰기 위한 파일 디스크립터로 변경
    }
    builtin_func(fd);
    if (cmd->end_type == 1) 
	{
        close(pipefd[1]); // 파이프 쓰기 닫음
        return pipefd[0]; // 파이프 읽기 파일 디스크립터 반환
    }
    return (0);
}


int execute_builtin(t_cmd *cmd, char ***env) 
{
    int fd = STDOUT_FILENO;
    int res;
    // 입력 리다이렉션 처리
    apply_redirection(cmd);
        // 내장 함수 실행
    if (ft_strncmp("pwd", cmd->av[0], 4) == 0)
        res = execute_builtin_with_pipe(cmd, ft_pwd, fd);
    // else if (ft_strncmp("export", cmd->av[0], 7) == 0)
    //     res = execute_builtin_with_pipe(cmd, ft_export, fd);
    // 다른 내장 함수들 추가
    else
        res = -1;
    return (res);
    }

int exec_cmd(t_cmd *cmd, char **env) 
{
    int pid;
    int has_pipe;
    int builtin_res;
    int in_fd = STDIN_FILENO;

    has_pipe = 0;
    builtin_res = execute_builtin(cmd, &env);
    if (builtin_res == -1) 
	{ // 외부 명령어인 경우
        if ((cmd->prev && cmd->prev->end_type == 1) || cmd->end_type == 1) 
		{
            pipe(cmd->end);
            has_pipe = 1;
            if (cmd->prev && cmd->prev->end_type == 1) // 이전 명령어가 존재, 이전 명령어 끝에 파이프 존재, 그 명령어가 내장함수일때
			{
				builtin_res = execute_builtin(cmd->prev, &env);
				if (builtin_res != -1)
					in_fd = builtin_res;
				else
					in_fd = cmd->prev->end[0];
			}
        }
        pid = fork();
        if (pid < 0) 
		{	
            perror("Error forking process");
            return 0;
        } 
		else if (pid == 0) 
            handle_child_process(cmd, env, in_fd);
        else 
			handle_parent_process(cmd, pid, has_pipe);
    }
    return (1);
}

void ft_execute(t_cmd *cmd, char **env)
{
	t_cmd	*temp;

	temp = cmd;
	while (temp)
	{
		exec_cmd(temp, env);
		temp = temp->next;
	}
}

int main()
{
    t_cmd cmd;
    char *env[] = {NULL};
    cmd.av = (char *[]){"pwd", NULL};
    cmd.end_type = 0; 
    cmd.redirect_input = 0;
    cmd.redirect_output = "outfile";
    cmd.append_flag = 0;
    cmd.prev = NULL;
    cmd.next = NULL;

    ft_execute(&cmd, env);
    return (0);
}
