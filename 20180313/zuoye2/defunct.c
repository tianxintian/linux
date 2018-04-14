#include"func.h"
int main()
{
	pid_t pid;
	pid = fork();
	if(pid==0)
	{
		printf("child,mypid=%d,myppid=%d\n",getpid(),getppid());
	}else{
		printf("parent,mypid=%d,mychildpid=%d\n",getpid(),pid);
		while(1);
	}
	return 0;
}
