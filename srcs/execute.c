

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
    int     in; //0(stdin), 1(infile), 2(heredoc)
    int     out; //0(stdout), 3(outfile), 4(append)
    char    *infile;
    char    *outfile;
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

int non_builtin(t_cmd *cmd, char **env, int fds[]) 
{
    char *full_path = get_full_path(env, cmd->av[0]);
    cmd->av[0] = full_path;

    int status;
    pid_t pid = fork();

    if (pid < -1) {
        perror("Error forking");
        exit(1);
    }

    if (pid == 0) 
    {
        if (cmd->end_type == 1 && cmd->out == 0)
            dup2(fds[1], 1);
        
        if (execve(cmd->av[0], cmd->av, env) == -1) 
        {
            perror("Error executing command");
            exit(1);
        }
    }
    else
        waitpid(pid, &status, 0);
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
    return (new_fd);
}

void right_redir(t_cmd *cmd)
{
    if (cmd->out == 3 && cmd->outfile) 
    {
        int flags = O_WRONLY | O_CREAT | O_TRUNC;
        int output_fd = open(cmd->outfile, flags, 0744);
        if (output_fd < 0) 
        {
            perror("Error opening output file");
            exit(1);
        }
        redirect_fd(output_fd, STDOUT_FILENO);
    }
    if (cmd->out == 4 && cmd->outfile) 
    {
        int flags = O_WRONLY | O_CREAT | O_APPEND;
        int output_fd = open(cmd->outfile, flags, 0744);
        if (output_fd < 0) 
        {
            perror("Error opening output file");
            exit(1);
        }
        redirect_fd(output_fd, STDOUT_FILENO);
    }
}

void left_redir(t_cmd *cmd)
{
    if (cmd->in == 1 && cmd->infile) 
    {
        int input_fd = open(cmd->infile, O_RDONLY);
        if (input_fd < 0) 
        {
            perror("Error opening input file");
            exit(1);
        }
        redirect_fd(input_fd, STDIN_FILENO);
    }
}

void apply_redir(t_cmd *cmd) 
{
    left_redir(cmd);
    right_redir(cmd);
}

void    ft_putstr_fd(char const *s, int fd)
{
    if (s == 0)
        return ;
    while (*s)
        write(fd, s++, 1);
}

int ft_pwd(int fd)
{
    char        *str;

    str = getcwd(NULL, 0);
    ft_putstr_fd(str, fd);
    write(fd, "\n", 1);
    free(str);
    return (1);
}

int exec_function(t_cmd *cmd, char ***env, int end[])
{
    int fd;
	if (cmd == NULL)
        return (-1);
    if (cmd->end_type == 1 && cmd->in != 0 || cmd->out != 0)
        apply_redir(cmd);
    if (cmd->end_type == 1 && cmd->in == 0 && cmd->out == 0)
        fd = end[1]; //output of the first cmd!!!!!!
    else
        fd = 1;
    if (ft_strncmp("pwd", cmd->av[0], 4) == 0)
        return (ft_pwd(fd));
    // else if (ft_strncmp("cd", cmd->av[0], 3) == 0)
    //     return (ft_cd(cmd));
    // else if (ft_strncmp("exit", cmd->av[0] 5) == 0)
    //     return (ft_exit(cmd));
    // else if (ft_strncmp("env", cmd->av[0], 4) == 0)
    //     return (ft_env(*env, fd));
    // else if (ft_strncmp("export", cmd->av[0], 7) == 0)
    //     return (ft_export(cmd, env, fd));
    // else if (ft_strncmp("echo", cmd->av[0], 5) == 0)
    //     return (ft_echo(cmd, fd));
    // else if (ft_strncmp("unset", cmd->av[0], 6) == 0)
    //     return (ft_unset(cmd, *env));
    else if (!non_builtin(cmd, *env, end)) 
        return (-1);
    return (0);
}

void exec(t_cmd *cmd, char **env)
{
    int     end[2];
    int     status;
    pid_t   pid;
    int     tmp;
    if(cmd == NULL)
    	return ;
    pipe(end);
    exec_function(cmd, &env, end);
    if (cmd->end_type == 1)
    {
        if ((pid = fork() < 0))
            perror("Error forking process");
    }
    if (pid == 0)
    {
        dup2(end[0], 0);
        close(end[1]);
        exec(cmd->next, env);
    }
    else
    {
        close(end[1]);
        close(end[0]);
        waitpid(pid, &status, 0);
    }

}

void exec_command_line(t_cmd *cmd, char **env)
{
    t_cmd   *temp;

    temp = cmd;
    while (temp)
    {
        exec(temp, env);
        temp = temp->next;
    }
}


t_cmd *new_cmd(char **av, int end_type, int in, int out, char *infile, char *outfile) {
    t_cmd *cmd = (t_cmd *)malloc(sizeof(t_cmd));
    if (cmd == NULL) 
    {
        perror("Error allocating memory for t_cmd");
        exit(1);
    }
    cmd->av = av;
    cmd->end_type = end_type;
    cmd->in = in;
    cmd->out = out;
    cmd->infile = infile;
    cmd->outfile = outfile;
    cmd->prev = NULL;
    cmd->next = NULL;
    return (cmd);
}


int main(int ac, char **av, char **env)
{
    (void)ac;
    (void)av;

    char *ls_cmd[] = {"ls", NULL};
    t_cmd *first_cmd = new_cmd(ls_cmd, 1, 0, 3, NULL, "outfile_1");

    char *pwd_cmd[] = {"pwd", NULL};
    t_cmd *second_cmd = new_cmd(pwd_cmd, 0, 0, 3, NULL, "outfile_2");

    first_cmd->next = second_cmd;
    second_cmd->prev = first_cmd;

    exec_command_line(first_cmd, env);

    t_cmd *temp;
    while (first_cmd)
    {
        temp = first_cmd;
        first_cmd = first_cmd->next;
        free(temp);
    }

    return (0);
}
