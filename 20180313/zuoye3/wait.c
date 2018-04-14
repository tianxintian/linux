#include"func.h"
int main()
{
	pid_t pid;
	pid=fork();
	if(pid==0)
	{
		printf("child,mypid=%d,myppid=%d\n",getpid(),getppid());
		return 7;
	}else
	{
		sleep(1);
		printf("parent,mypid=%d,mychildpid=%d\n",getpid(),pid);
		pid_t cpid;
		cpid=wait(NULL);
		printf("cpid=%d\n",cpid);
		return 0;
	}
}
