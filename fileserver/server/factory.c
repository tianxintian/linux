#include"factory.h"
void factory_init(pfac p,pfunc thread_func,int cap)
{//初始化这个管理工厂，管理线程申请与描述符队列的初始化
//并且指定线程处理数
	p->pthid=(pthread_t*)calloc(p->pthread_num,sizeof(pthread_t));
	pthread_cond_init(&p->cond,NULL);
	que_init(&p->que,cap);
	p->thread_func=thread_func;
}
void factory_start(pfac p)//建立线程
{
	if(!p->start_flag)
	{
		int i;
		for(i=0;i<p->pthread_num;i++)
		{
			pthread_create(p->pthid+i,NULL,p->thread_func,p);
		}
		p->start_flag=1;
	}
}








