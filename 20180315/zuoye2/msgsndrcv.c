#include"func.h"
typedef struct msgbuf{
	long mtype;
	int  mtext[100];
}data;
int main()
{
	int msgid=msgget(1000,0600|IPC_CREAT);
	if(msgid==-1)
	{
		perror("msgget");
		return -1;
	}
	data t;
	int ret;
	if(fork())
	{
		int i=50;
		t.mtype=1;
		memcpy(t.mtext,&i,sizeof(int));
		ret=msgsnd(msgid,&t,4,0);
		if(ret==-1)
		{
			perror("msgsnd");
			return -1;
		}
		wait(NULL);
		ret=msgctl(msgid,IPC_RMID,NULL);
		if(ret==-1)
		{
			perror("msgctl");
			return -1;
		}
	}else
	{
		sleep(10);
		bzero(&t,sizeof(t));
		t.mtype=1;
		ret=msgrcv(msgid,&t,sizeof(t.mtext),1,0);
		if(ret==-1)
		{
			perror("msgrcv");
			return -1;
		}
		printf("%d\n",*(t.mtext));
		return 0;
	}
	return 0;
}







































