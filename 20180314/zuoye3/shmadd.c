#include"func.h"
#define N 10000000
int main()
{
	int shmid;
	shmid=shmget(777,4096,0600|IPC_CREAT);
	if(shmid==-1)
	{
		perror("shmget");
		return -1;
	}
	int *p=(int*)shmat(shmid,NULL,0);
	if(p==(int*)-1)
	{
		perror("shmat");
		return -1;
	}
	p[0]=0;
	int i;
	if(!fork())
	{
		for(i=0;i<N;i++)
		{
			p[0]=p[0]+1;
		}
	}else
	{
		for(i=0;i<N;i++)
		{
			p[0]=p[0]+1;
		}
		wait(NULL);
		printf("p[0]=%d\n",p[0]);
	}
	return 0;
}
















