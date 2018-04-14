#include"func.h"
void* threadfunc(void* p)
{
	printf("value is %ld\n",(long)p);
	return(void*)2;
}
int main()
{
	pthread_t pthid;
	int ret;
	ret=pthread_create(&pthid,NULL,threadfunc,(void*)1);
	if(ret!=0)
	{
		printf("ret is %d\n",ret);
		return -1;
	}
	long num;
	ret=pthread_join(pthid,(void**)&num);
	if(ret!=0)
	{
		printf("join ret is %d\n",ret);
		return -1;
	}
	printf("num is %ld\n",num);
	exit(0);
}


