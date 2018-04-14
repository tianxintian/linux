#include"func.h"
#define N 10000000
int main(int argc,char*argv[])
{
	int shmid=shmget(777,4096,0600|IPC_CREAT);
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
	int sem_id=semget(777,1,0600|IPC_CREAT);
	if(sem_id==-1)
	{
		perror("semget");
		return -1;
	}
	int ret=semctl(sem_id,0,SETVAL,1);
	if(ret==-1)
	{
		perror("semctl");
		return -1;
	}
	struct sembuf sopp,sopv;
	sopp.sem_num=0;
	sopp.sem_op=-1;
	sopp.sem_flg=SEM_UNDO;
	sopv.sem_num=0;
	sopv.sem_op=1;
	sopv.sem_flg=SEM_UNDO;
	p[0]=0;
	int i;
	if(!fork())
	{
		for(i=0;i<N;i++)
		{
			semop(sem_id,&sopp,1);
			p[0]=p[0]+1;
			semop(sem_id,&sopv,1);
		}
	}else
	{
		for(i=0;i<N;i++)
		{
			semop(sem_id,&sopp,1);
			p[0]=p[0]+1;
			semop(sem_id,&sopv,1);
		}
		wait(NULL);
		printf("p[0]=%d\n",p[0]);
		ret =shmctl(shmid,IPC_RMID,NULL);
		if(ret==-1)
		{
			perror("shmctl");
			return -1;
		}
		ret=semctl(sem_id,0,IPC_RMID);
		if(ret==-1)
		{
			perror("semctl");
			return -1;
		}
	}
	return 0;
}









































