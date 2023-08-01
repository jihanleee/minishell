#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

//av 로 받은 인자들을 "|" 나 ";" 으로 끊어서 struct 에 넣고 연결 리스트로 이어붙힘.
// av 돌면서 "|" ";"



typedef struct s_cmd
{
    int     cnt_lexeme; // 명령어 묶음 개수
    char    **av; //파이프랑  ; 전 까지 끊어서 저장해두고 execve 에 통째로 넣어줌.
    int     end_type;
    int     end[2];
    struct  s_cmd *prev; // 파이프 중간에 있는지 확인해야하니까
    struct  s_cmd *next;
}   t_cmd;


void	ft_putstr_fd(char const *s, int fd)
{
	if (s == 0)
		return ;
	while (*s)
		write(fd, s++, 1);
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

char *ft_strdup(char *s)
{
	int len = ft_strlen(s);
	char *new;

	if (!s)
		return (NULL);
	if (!(new = (char *)malloc(sizeof(char) * (len + 1))))
		return (NULL);
	new[len] = '\0';
	while (--len >= 0)
		new[len] = s[len];
	return (new);
}

void    ft_lst_add_back(t_cmd **cmd, t_cmd *new)
{
	t_cmd	*temp;

	if (!(*cmd))
		*cmd = new;
	else
	{
		temp = *cmd;
		while (temp->next)
			temp = temp->next;
		temp->next = new;
		new->prev = temp;
	}
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

void handle_child_process(t_cmd *cmd, char **env, int in_fd) 
{
    char *full_path;

    // 입출력 리다이렉션 처리
    if (cmd->prev && cmd->prev->end_type == 1 && dup2(cmd->prev->end[0], STDIN_FILENO) < 0) 
	{
        perror("Error duplicating file descriptor");
        exit(1);
    }
    if (in_fd != STDIN_FILENO) 
	{
        if (dup2(in_fd, STDIN_FILENO) < 0) {
            perror("Error duplicating file descriptor");
            exit(1);
        }
    }
    if (cmd->end_type == 1 && dup2(cmd->end[1], STDOUT_FILENO) < 0)
    {
        perror("Error duplicating file descriptor");
        exit(1);
    }
    full_path = get_full_path(env, cmd->av[0]);
    cmd->av[0] = full_path;
    if (execve(cmd->av[0], cmd->av, env) < 0) 
	{
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
    return (-1);
}

int execute_builtin(t_cmd *cmd, char ***env) {
    int fd = STDOUT_FILENO;
    int res;

    if (ft_strncmp("pwd", cmd->av[0], 4) == 0)
        res = execute_builtin_with_pipe(cmd, ft_pwd, fd);
    // else if (ft_strncmp("cd", cmd->av[0], 3) == 0)
    //     res = execute_builtin_with_pipe(cmd, ft_cd, fd);
    // else if (ft_strncmp("export", cmd->av[0], 7) == 0)
    //     res = execute_builtin_with_pipe(cmd, ft_export, fd);
    // else if (ft_strncmp("env", cmd->av[0], 4) == 0)
    //     res = execute_builtin_with_pipe(cmd, ft_env, fd);
    // else if (ft_strncmp("unset", cmd->av[0], 6) == 0)
    //     res = execute_builtin_with_pipe(cmd, ft_unset, fd);
    else
        res = -1;
return (res);
}

// exec_cmd 함수에서 외부 명령어 실행 부분
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

int parse_cmd(t_cmd **cmd, char **av)
{
	t_cmd	*new;
	int		cnt_lexeme;

	if (!(new = (t_cmd *)malloc(sizeof(t_cmd))))
		return (0);
////////////////struct 에 넣을 인자 갯수 확인/////////////////
	int i = 0;
	while (av[i] && strcmp(av[i], ";") != 0 && strcmp(av[i], "|") != 0)
		i++;
////////////////////cnt_lexeme 초기화, av 끝에 null ///////////////////
    if (!(new->av = (char **)malloc(sizeof(char *) * (i + 1))))
		return (0);
	new->cnt_lexeme= i;
	new->av[cnt_lexeme] = NULL;
//////////////////struct 에 "|" , ";" 전까지 나온 인자 strdup///////////
	while (--i >= 0)
		new->av[i] = strdup(av[i]);
////////////////////// 뒤에 파이프인제 세미콜론인지 확인////////////////
	if (!av[new->cnt_lexeme])
		new->end_type = 0; 
	else if (strcmp(av[new->cnt_lexeme],"|") == 0)
		new->end_type = 1; //define 으로 PIPE = 1 해주기
	else if (strcmp(av[new->cnt_lexeme],";") == 0)
		new->end_type = 2; // 마찬가지 define 으로 semi_col = 2;

//////////////////////prev, next//////////////////////////////////////
	new->prev = NULL;
	new->next = NULL;
	ft_lst_add_back(cmd, new); //cmd 에 넣어줌
	return (new->cnt_lexeme);
}

int main(int ac, char **av, char **env)
{
	t_cmd *cmd;
	int i;

	if (ac < 1)
		return (0); // 에러 메세지 출력하기
	i = 1;
	cmd = NULL;
	while (av[i])
	{
		i += parse_cmd(&cmd, &av[i]); // 파싱해서 cmd 에 저장
		if (!av[i]) // 다음 인자 없으면 여기서 파싱 끝
			break;
		i++;
	}
	if (cmd)
		ft_execute(cmd, env);
    //ft_free(cmd); 나중에
	return (0);
}
