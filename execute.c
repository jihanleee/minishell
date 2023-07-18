
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
    int     cnt_word; // 명령어 묶음 개수
    char    **av; //파이프랑  ; 전 까지 끊어서 저장해두고 execve 에 통째로 넣어줌.
    int     end_type;
    int     end[2];
    struct  s_cmd *prev; // 파이프 중간에 있는지 확인해야하니까
    struct  s_cmd *next;
}   t_cmd;


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
    t_cmd *temp;

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

/*
1. ls -al ===> 명령어 하나 (prev 없고 next 없을때) -> fork만
2. ls -al ; echo hello ===> end type 세미 콜론일, cmd->next 존재 fork 만 해줌 
3. ls -al | wc -l ===> end_type 파이프, cmd->next 있을때-> fork, pipe
4. ls -al | wc -l | grep hi ===> 이전 endtype 도 파이프 현재 endtype 도 파이프
*/

int    exec_cmd(t_cmd *cmd, char **env)
{
    int status;
    int pid;
    int has_pipe;

    has_pipe = 0;
    if (!cmd->prev && !cmd->next)  // 1번, 3번인 경우 = 포크만
        pid = fork();
    else if ((cmd->prev && cmd->prev->end_type == 1) || cmd->end_type == 1) // 2번인 경우 = 포크, 파이프 생성
    {
        pid = fork();
        pipe(cmd->end);
        has_pipe = 1;
    }
    
    if (pid < 0)
        return (0); // 파이프 실패 에러 메세지 출력

    else if (pid == 0) // 자식 프로세스
    {
        if (cmd->end_type == 1 && dup2(cmd->end[1], STDOUT_FILENO ) < 0 ) //파이프 write end 가 stdout 으로 dup 되어 대체 됨.
            return (-1); //에러 출력
        else if (cmd->prev && cmd->prev->end_type == 1 && dup2(cmd->prev->end[0], 0) < 0) //파이프 read end 가 stdin 으로 dup 되어 대체 됨. 첫번째 두번째 proc 파이프 연결 완료! 
            return (-1); //에러 출력

    	if (execve(cmd->av[0], cmd->av, env) < 0) //=> 여기서 실행해줌.
            return (-1); //에러 출력쓰
    }

    else // 부모 프로세스는 적절하게 if 문으로 파이프 fd 둘 다 닫아버리면 끝
    {
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

}


void ft_execute(t_cmd *cmd, char **env)
{
	t_cmd *temp;

	temp = cmd;
	while (temp)
	{
		// if (strcmp("cd", temp->argv[0]) == 0)  /// 나중에 빌트인 함수 넣어서 진행
		// {

		// }
        // if (strcmp("echo", temp->argv[0]) == 0)
        // {

        // } 

		//else                                      //// 이 외 함수
		    exec_cmd(temp, env);
		temp = temp->next;
	}
}

int parse_cmd(t_cmd **cmd, char **av) // malloc 은 내일...
{
    t_cmd *new;
    int cnt_word;

    if (!(new = (t_cmd *)malloc(sizeof(t_cmd))))
        return (0);
////////////////struct 에 넣을 인자 갯수 확인/////////////////
    int i = 0;
    while (av[i])
    {
        if (av[i] == ";" || av[i] == "|")
            break;
        i++;
    }
////////////////////cnt_word 초기화, av 끝에 null ///////////////////
    if (!(new->av = (char **)malloc(sizeof(char *) * (i + 1))))
        return (0);
    new->cnt_word= i;
    new->av[i] = NULL;
//////////////////struct 에 "|" , ";" 전까지 나온 인자 strdup///////////
    while (--i >= 0)
        new->av[i] = strdup(av[i]);
////////////////////// 뒤에 파이프인제 세미콜론인지 확인////////////////
    if (av[new->cnt_word] == "|")
        new->end_type = 1; //define 으로 PIPE = 1 해주기
    else if (av[new->cnt_word] == ";")
        new->end_type = 2; // 마찬가지 define 으로 semi_col = 2;
    else
        new->end_type = 0; // 아무것도 없을 때 define 뭘로하지
//////////////////////prev, next//////////////////////////////////////
    new->prev = NULL;
    new->next = NULL;
    ft_lst_add_back(cmd, new); //cmd 에 넣어줌
    return (new->cnt_word);
}

int main(int ac, char **av, char **env)
{
    t_cmd *cmd;
    int i;
    
    if (ac < 1)
        return (0); // 에러 메세지 출력하기
    i = 1;
    cmd = NULL;
    while (av[1])
    {
        i += parse_cmd(&cmd, &av[1]); // 파싱해서 cmd 에 저장
        if (!av[i]) // 다음 인자 없으면 여기서 파싱 끝
            break;
        i++;
    }
    if (cmd)
        ft_execute(cmd, env);
    //ft_free(cmd); 나중에
}
