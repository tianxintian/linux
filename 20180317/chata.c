#include"func.h"
typedef struct myaa{
	int flag;
	char buf[512];
}mya;
int fdr,fdw;
pid_t pid;
int sem_id;
int shmid;
void function()
{
	shmctl(shmid,IPC_RMID,NULL);
	semctl(sem_id,0,IPC_RMID);
	close(fdr);
	close(fdw);
	kill(pid,SIGKILL);
}
int main(int argc,char*argv[])
{
	shmid=shmget(777,4096,0600|IPC_CREAT);
	pid=getpid();
struct myaa *p=(struct myaa*)shmat(shmid,NULL,0);
	mya my;
    sem_id=semget(777,1,0600|IPC_CREAT);
	semctl(sem_id,0,SETVAL,1);
	struct sembuf sopp,sopv;
	sopp.sem_num=0;
	sopp.sem_op=-1;
	sopp.sem_flg=SEM_UNDO;
	sopv.sem_num=0;
	sopv.sem_op=1;
	sopv.sem_flg=SEM_UNDO;
	if(argc!=3)
	{
		printf("error args\n");
		return -1;
	}
	fdr=open(argv[1],O_RDONLY);
	if(fdr==-1)
	{
		perror("open1");
	}
	fdw=open(argv[2],O_WRONLY);
	if(fdw==-1)
	{
		perror("open2");
		return -1;
	}
	int ret;
	fd_set rdset;
	while(1)
	{
		signal(SIGINT,function);
		FD_ZERO(&rdset);
		FD_SET(0,&rdset);
		FD_SET(fdr,&rdset);
		ret=select(fdr+1,&rdset,NULL,NULL,NULL);
		if(ret>0)
		{
			if(FD_ISSET(fdr,&rdset))
			{
				bzero(p->buf,sizeof(p->buf));
				bzero(my.buf,sizeof(my.buf));
				ret=read(fdr,my.buf,sizeof(my.buf));
				if(ret==-1)
				{
					perror("read");
					return 0;
				}else if(ret==0)
				{
					printf("byebye\n");
					function();
					break;
				}
				semop(sem_id,&sopp,1);
				memcpy(p->buf,my.buf,strlen(my.buf));
				my.flag=1;
				p->flag=my.flag;
				semop(sem_id,&sopv,1);
				printf("%s\n",my.buf);
			}
			if(FD_ISSET(0,&rdset))
			{
				bzero(p->buf,sizeof(p->buf));
				bzero(my.buf,sizeof(my.buf));
				ret=read(0,my.buf,sizeof(my.buf)-1);
				if(ret==0)
	   			{
					printf("end read\n");
					break;
				}
				semop(sem_id,&sopp,1);
				memcpy(p->buf,my.buf,strlen(my.buf));
				my.flag=2;
				p->flag=my.flag;
				semop(sem_id,&sopv,1);	
				write(fdw,my.buf,strlen(my.buf)-1);
			}
		}
	}
	return 0;
}

















































