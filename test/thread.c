#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
//线程的执行代码段（线程的执行函数）
int *thread_handler(void *arg){
	int a = *((int *)arg);
	while(a > 0){
		printf("thread_handler\n");
		sleep(1);
		a--;
	}

	pthread_exit("thread");
}
int main(int argc, const char *argv[])
{
	pthread_t thread;
	int i = 5;
	void *retval;
	if((pthread_create(&thread, NULL, (void *)thread_handler, (void *)&i)) != 0){
		perror("pthread_create");
		return -1;
	}
#if 0
	for(i = 0; i < 3; i++){
		printf("main\n");
		sleep(1);
	}
#endif
	//阻塞等待线程退出
	pthread_join(thread, &retval);

	printf("%s\n", (char *)retval);
	return 0;
}