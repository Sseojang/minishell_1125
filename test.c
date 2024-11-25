#include "ms_test.h"

void	ft_paser_func(t_tokken_list **tokken, t_val *val, int *pipefd)
{
	ft_val_set(*tokken, val);
	ft_find_pipe(*tokken, val, pipefd);
	ft_find_redir(tokken, val);
	ft_find_cmd(*tokken, val);
}

void ft_paser_manager(t_tokken_list *tokken, char **envp)
{
    pid_t pid;
    t_val val;
    int pipefd[2] = {-1, -1};
    int prev_pipe = -1;
    int status;

    while (tokken)
    {
        // 다음 토큰이 있을 때만 파이프 생성
        if (ft_next_pipe(tokken))
        {
            if (pipe(pipefd) == -1)
                error("pipe failed", 1);
        }
        else
        {
            pipefd[0] = -1;
            pipefd[1] = -1;
        }

        ft_paser_func(&tokken, &val, pipefd);
        pid = fork();
        if (pid < 0)
            error("Fork error", 1);
        else if (pid == 0)
        {
            // 자식 프로세스
            if (prev_pipe != -1)
            {
                dup2(prev_pipe, STDIN_FILENO);
                close(prev_pipe);
            }
            if (pipefd[1] != -1)
            {
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[1]);
            }
            if (pipefd[0] != -1)
                close(pipefd[0]);
            
            // 단일 명령일 때는 NULL을 전달
            ft_dup(&val, envp, tokken->next ? pipefd : NULL);
            exit(1); // ft_dup에서 실행에 실패한 경우
        }
        else
        {
            // 부모 프로세스
            if (prev_pipe != -1)
                close(prev_pipe);
            if (pipefd[1] != -1)
                close(pipefd[1]);
            prev_pipe = pipefd[0];
        }
		while (tokken && tokken->content && ft_strncmp(tokken->content, "|", 1) != 0)
		{
			printf("test here : {%s}\n", tokken->content);
            tokken = tokken->next;
		}
        if (tokken && tokken->content && ft_strncmp(tokken->content, "|", 1) == 0)
            tokken = tokken->next;
        //tokken = tokken->next;
    }

    // 마지막 파이프 닫기
    if (prev_pipe != -1)
        close(prev_pipe);

    // 모든 자식 프로세스가 종료될 때까지 대기
    while (wait(&status) > 0);
}