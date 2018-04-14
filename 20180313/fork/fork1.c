#include"func.h"
int main()
{
	pid_t pid;
	pid = fork();
	if(pid==0)
	{
		printf("erzi\n");
	}else
	{
		printf("fuqin\n");
		sleep(1);
	}
	printf("haha\n");
	return 0;
}
