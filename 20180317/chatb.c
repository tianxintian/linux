#include"func.h"
typedef struct msgbuf{
	long mtype;
	char mtext[128];
}data;
int fdr,fdw;
int msgid;
pid_t pid;
int sem_id;
void function()
{
	msgctl(msgid,IPC_RMID,NULL);
	semctl(sem_id,0,IPC_RMID);
	close(fdr);
	close(fdw);
	kill(pid,SIGKILL);
}
int main(int argc,char*argv[])
{
	pid=getpid();
	if(argc!=3)
	{
		printf("error args\n");
		return -1;
	}
	int fdr,fdw;
	fdw=open(argv[1],O_WRONLY);
	if(fdw==-1)
	{
		perror("open1");
		return -1;
	}
	fdr=open(argv[2],O_RDONLY);
	if(fdr==-1)
	{
		perror("open");
		return -1;
	}
	msgid=msgget(1000,0600|IPC_CREAT);
	sem_id=semget(710,1,0600|IPC_CREAT);
	semctl(sem_id,0,SETVAL,1);
	struct sembuf sopp,sopv;
	sopp.sem_num=0;
	sopp.sem_op=-1;
	sopp.sem_flg=SEM_UNDO;
	sopv.sem_num=0;
	sopv.sem_op=1;
	sopv.sem_flg=SEM_UNDO;
	char buf[128]={0};
	int ret;
	fd_set rdset;
	data t;
	t.mtype=7;
	while(1)
	{
		signal(SIGINT,function);
		FD_ZERO(&rdset);
		FD_SET(0,&rdset);
		FD_SET(fdr,&rdset);
		ret=select(fdr+1,&rdset,NULL,NULL,NULL);
		if(ret>0)
		{
			if(FD_ISSET(0,&rdset))
			{
				bzero(buf,sizeof(buf));
				ret=read(0,buf,sizeof(buf)-1);
				if(ret==0)
				{
					printf("end read\n");
					function();
					break;
				}
				semop(sem_id,&sopp,1);
				t.mtype=1;
				bzero(t.mtext,sizeof(t.mtext));
				strcpy(t.mtext,buf);
				msgsnd(msgid,&t,128,0);
				write(fdw,buf,strlen(buf)-1);
				semop(sem_id,&sopv,1);
			}
			if(FD_ISSET(fdr,&rdset))
			{
				bzero(buf,sizeof(buf));
				ret=read(fdr,buf,sizeof(buf));
				if(ret==0)
				{
					function();
					printf("byebye\n");
					break;
				}
				semop(sem_id,&sopp,1);
				t.mtype=2;
				bzero(t.mtext,sizeof(t.mtext));
				strcpy(t.mtext,buf);
                strcat(t.mtext,"\n");
				msgsnd(msgid,&t,128,0);
				semop(sem_id,&sopv,1);
				printf("%s\n",buf);
			}
		}
	}
	return 0;
}










































