#include"func.h"
void cleanup(void *p)
{
	free(p);
	printf("cleaned!\n");
}
void* threadfunc(void *args)
{
	char*p=(char*)malloc(7);
	pthread_cleanup_push(cleanup,p);
	read(0,p,sizeof(p));
	pthread_cleanup_pop(1);
}
int main()
{
	pthread_t pthid;
	pthread_create(&pthid,NULL,threadfunc,NULL);
	int ret;
	sleep(3);
	ret=pthread_cancel(pthid);
	if(ret!=0)
	{
		printf("cacelret is %d\n",ret);
		return -1;
	}
	pthread_join(pthid,NULL);
	exit(0);
}

