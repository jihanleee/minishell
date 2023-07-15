
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

//av 로 받은 인자들을 "|" 나 ";" 으로 끊어서 struct 에 넣고 연결 리스트로 이어붙힘.
// av 돌면서 "|" ";" ㅁ


typedef struct s_cmd
{
    int     cnt_word;    
    char    **av; //파이프랑  ; 전 까지 끊어서 저장해두고 execve 에 통째로 넣어줌.
    int     end_type;
    int     fd[2];
    struct  s_cmd *prev; // 파이프 중간에 있는지 확인해야하니까
    struct  s_cmd *next;
}   t_cmd;

char *ft_strdup(char *s1, char *s2)
{
    //ㅇㅇ
}

void    ft_lst_add_back(t_cmd **cmd, t_cmd *new)
{
    //만들기
}

char **parse_cmd(t_cmd *cmd, char **av) //여기서 초기화 시키고 struct 저장.
{
    t_cmd *new;
    int cnt_word;
    int i = 0;
////////////////struct 에 넣을 인자 갯수 확인/////////////////
    while (av[i])
    {
        if (av[i] == ";" || av[i] == "|")
            break;
        i++;
    }
////////////////////cnt_word 초기화, av 끝에 null ///////////////////
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

/*
1. ls -al 명령어 하나 (prev 없고 next 없을때) -> fork만
2. ls -al | wc -l : end_type 파이프, cmd->next 있을때-> fork, pipe
3. ls -al ; echo hello : end type 세미 콜론일, cmd->next 존재 fork 만 해줌 */
void    ft_execute(t_base *cmd, char **env)
{
    int pid;
    int has_pipe = 0;

    if (!cmd->prev && !cmd->next) ||  // 1번, 3번인 경우 = 포크만
        pid = fork();
    else if (cmd->next && end_type == 1) // 2번인 경우 = 포크, 파이프 생성
    {
        pid = fork();
        pipe(cmd->pipe);
        has_pipe = 1;
    }
    
    if (pid < 0)
        return (0) // 파이프 실패 에러 메세지 출력


/*
if has pipe 안에 들어있는 조건문


1) prev->type end 가 ; 이거나 || 파이프가 아니거나 || !cmd->prev 일때
ls -al | wc -l 
echo hello ; ls -al | wc -l

ls -al

dup2(pipefd[1], STDOUT); 파이프 write end 가 stdout 으로 dup 되어 대체 됨.
close(pipefd[1]); 이미 stdout 으로 대체 되어서 닫아버림
close(pipefd[0]; 안씀

2) prev->type end 파이프, 본인 type end 파이프 (두번째/ 중간에 껴있는 명령어)
ls -al | wc -l | grep hi  (wc -l 같은 경우)

cmd->prev 의 type end 가 파이프, !cmd->next || cmd -> ;
close(pipefd[1]); 안씀
dup2(pipefd[0], STDIN); 파이프 read end 가 stdin 으로 dup 되어 대체 됨.
close(pipefd[0]);

close(pipefd[0]);
dup2(pipefd[1], STDOUT);
close(pipefd[1]);


3. prev->type end 파이프 && 본인 type end != pipe 일때 마지막 명령어
ls -al | wc -l | grep hi (grep hi 의 경우) 

첫번째랑 같음

그니까 1,3번째 케이스 같이 묶고 두번째만 따로 뺌.

*/
    else if (pid == 0) // 자식 프로세스
    {
        if (has_pipe)
        {
            if (cmd->prev->end_type == 1 && cmd->next)
            {
                close(pipefd[1]); 안씀
                dup2(pipefd[0], STDIN); 파이프 read end 가 stdin 으로 dup 되어 대체 됨.
                close(pipefd[0]);
                
                close(pipefd[0]);
                dup2(pipefd[1], STDOUT);
                close(pipefd[1]);
            }
            else
            {
                dup2(cmd->fd[1], STDOUT_FILENO); //파이프 write end 가 stdout 으로 dup 되어 대체 됨.
                close(cmd->fd[1]); //이미 stdout 으로 대체 되어서 닫아버림
                close(cmd->fd[0]); //안씀
            }
    		execve(cmd->argv[0], cmd->av, env) => 여기서 실행해줌.


        }

    }

    else // 부모 프로세스는 적절하게 if 문으로 파이프 fd 둘 다 닫아버리면 끝
    {
        waitpid(pid, &status, 0);
        if (has_pipe)
        {
           // close close close
        }

    }

}

int main(int ac, char **av)
{
    t_cmd *cmd;
    int i;
    
    if (ac < 1)
        return (0) // 에러 메세지 출력하기
    i = 1;
    ptr = NULL;
    while (av[1])
    {
        i += parse_cmd(&cmd, av[1]); // 파싱해서 ptr 에 저장
        if (!av[i]) // 다음 인자 없으면 여기서 파싱 끝
            break;
        i++;
    }
    if (ptr)
        ft_execute(ptr, env);
    ft_free(ptr);
}