#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>

static pid_t __pid ;

void child()
{
    switch(fork())
    {
        case -1:
            exit(-1);
            break;
        case 0:
            break;
        default:
            exit(0);
    }

    close(0);
    close(1);
    close(2);
    open("/dev/null", O_RDWR);//0
    dup2(1,2);
    printf("守护进程PID=%d 守护进程父进程PID=%d\n",getpid(),getppid());
}

static void _watch_on_child(void)
{
	int status;
START:
	__pid = fork();
    printf("__pid = %d\n",__pid);
    
	if( __pid > 0)
	{
        printf("%s %d 当前进程PID=%d 子进程PID=%d\n",__FILE__,__LINE__,getpid(),__pid);
        printf("status=%d\n",status);
		//this is parrent
		waitpid(__pid, &status, 0); //第三个参数0，阻塞等待子进程退出
		if( status == 0 )
		{
			/* child normal exit, parent should exit too */
			printf("child exit nomal\n");
			exit(0);
		}
		else if(WIFSIGNALED(status)) //exit by signal  如果因为信号退出则为真
		{
            printf("test1--------\n");
			//if child exit by kill or kill -9, parten should exit too
			if(WTERMSIG(status) == 15 || WTERMSIG(status) == 9) //kill不加参数默认15
            {
                printf("test1--------\n");
                exit(0);
            }		
		}
		printf("restart status = %d\n",status);
		goto START; //exit unnomal, start again
	}
    
    return;
}
int main(int argc, char const *argv[])
{
    //child();
    _watch_on_child();
    printf("%s %d main父进程PID=%d main进程PID=%d\n",__FILE__,__LINE__,getppid(),getpid());
    while(1)
    {
        sleep(2);
        printf("hello\n");
    }
    return 0;
}
