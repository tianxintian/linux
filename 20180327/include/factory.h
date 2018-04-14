#ifndef __FACTORY_H__
#define __FACTORY_H__
#include"head.h"
#include"work_que.h"
typedef void* (*pfunc)(void*);
typedef struct{
	pthread_t *pthid;//线程id存储的起始地址
	pthread_cond_t cond;//控制条件变量
	que_t que;//队列变量
	int start_flag;//启动标志
	int pthread_num;//线程的数量
	pfunc thread_func;//线程处理函数
}fac,*pfac;
typedef struct{
	int len;//控制传输的长度
	char buf[1000];
}train;
#define FILENAME "file.avi"
void factory_init(pfac,pfunc,int);
void factory_start(pfac);
int send_n(int,char*,int);
int recv_n(int,char*,int);
void sendfile(int);
#endif












