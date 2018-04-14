#include"func.h"
typedef struct mya{
	pthread_mutex_t mutex;
	pthread_cond_t cond;
}data;
void* threadfunc(void*p)
{
	data* t=(data*)p;
	printf("wait\n");
	pthread_mutex_lock(&t->mutex);
	int ret=pthread_cond_wait(&t->cond,&t->mutex);
	pthread_mutex_unlock(&t->mutex);
	printf("wakeup ret=%d\n",ret);
}
int main()
{
	pthread_t pthid;
	data t;
	pthread_mutex_init(&t.mutex,NULL);
	int ret=pthread_cond_init(&t.cond,NULL);
	if(ret!=0)
	{
		printf("condinitret=%d\n",ret);
		return -1;
	}
	pthread_create(&pthid,NULL,threadfunc,&t);
	usleep(2000);
	pthread_cond_signal(&t.cond);
	printf("send signal success\n");
	pthread_join(pthid,NULL);
	return 0;
}
