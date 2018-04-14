#include"func.h"
typedef struct msgbuf{
	long mtype;
	char mtext[128];
}data;
int main()
{
	data t;
	t.mtype=7;
	while(1)
	{
		int msgid=msgget(1000,0600|IPC_CREAT);
		int ret=msgrcv(msgid,&t,sizeof(t.mtext),0,0);
		if(ret==-1)
		{
			break;
		}
		if(t.mtype==1)
		{
			printf("B:%s",t.mtext);
		}
		if(t.mtype==2)
		{
			printf("\t\t\t\t A:%s",t.mtext);
		}
	}
	return 0;
}
