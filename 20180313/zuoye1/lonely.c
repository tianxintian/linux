#include"func.h"
int main()
{
	pid_t pid;
	pid=fork();
	if(pid==0)
	{
		printf("haizi,mymypid=%d,myppid=%d\n",getpid(),getppid());
		while(1);
	}else
	{
		printf("parent,mypid=%d,myppid=%d\n",getpid(),pid);
	}
	return 0;
}




