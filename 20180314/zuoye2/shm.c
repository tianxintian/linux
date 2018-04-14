#include"func.h"
int main()
{

	int shmid;
	shmid=shmget(777,4096,0600|IPC_CREAT);
	if(shmid==-1)
	{
		perror("shmget");
		return -1;
	}
	if(!fork())
	{
		char *p=(char*)shmat(shmid,NULL,0);
		if((char *)-1==p)
		{
			perror("shmat");
			return -1;
		}
		strcpy(p,"how are you.");
	}else
	{
		sleep(3);
		char *pr=(char*)shmat(shmid,NULL,0);
		printf("%s\n",pr);
		wait(NULL);
	}
	exit(0);
}
