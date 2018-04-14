#include"func.h"
typedef struct myaa{
	int flag;
	char buf[512];
}mya;
int main()
{
	int ret;
    int shmid=shmget(777,4096,0600|IPC_CREAT);
	struct myaa* p=(struct myaa*)shmat(shmid,NULL,0);
	while(1)
	{
		if((p->flag)==1)
		{
			printf("\t\t\t\t B:%s\n",p->buf);
			p->flag=0;
		}
		if((p->flag)==2)
		{
			printf("A:%s",p->buf);
			p->flag=0;
		}
	}
	return 0;
}



























