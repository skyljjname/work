#include <stdio.h>
#include <unistd.h>

int main(int argc, const char *argv[])
{
	pid_t pid;

//	printf("hello world\n");
	int a = 5;
	pid = fork();
    printf("pid=%d\n",pid);
	if(pid < 0){
		perror("fork error");
		return -1;
	}
	else if(pid == 0){
		//child
		int a = 3;
		printf("child, a = %d &a = %p\n",a , &a);
		printf("子进程ID = %d 子进程父进程ID = %d\n", getpid(), getppid());
		while(1);
	}
	else{
		//parent
		printf("parent, a = %d &a = %p\n", a, &a);
		printf("父进程PID = %d 父进程ID = %d\n", pid, getpid());
		while(1);
	}
	return 0;
}
