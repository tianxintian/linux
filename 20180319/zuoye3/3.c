#include"func.h"
#define N 20000000
typedef struct mya{
	pthread_mutex_t mutex;
	long num;
}data;
void *threadfunc(void* p)
{
	data* t=(data*)p;
	int i;
	for(i=0;i<N;i++)
	{
		pthread_mutex_lock(&t->mutex);
		t->num=t->num+1;
		pthread_mutex_unlock(&t->mutex);
	}
}
int main()
{
	pthread_t pthid;
	data t;
	t.num=0;
	int ret;
	struct timeval ts,te;
	ret=pthread_mutex_init(&t.mutex,NULL);
	if(ret!=0)
	{
		printf("mutexret is %d\n",ret);
		return -1;
	}
	gettimeofday(&ts,NULL);
	ret=pthread_create(&pthid,NULL,threadfunc,&t);
	if(ret!=0)
	{
		printf("createret is %d\n",ret);
		return -1;
	}
	int i;
	for(i=0;i<N;i++)
	{
		pthread_mutex_lock(&t.mutex);
		t.num=t.num+1;
		pthread_mutex_unlock(&t.mutex);
	}
	pthread_join(pthid,NULL);
	gettimeofday(&te,NULL);
	printf("sum is %ld\nconsumetime is %ldus\n",t.num,(te.tv_sec-ts.tv_sec)*1000000+te.tv_usec-ts.tv_usec);
	pthread_mutex_destroy(&t.mutex);
	exit(0);
}























