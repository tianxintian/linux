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
	int pthread_num;//线程的数
	pfunc thread_func;//线程处理函数
//	char bufip[77];//存储线程连接ip
}fac,*pfac;
typedef struct{
	int len;//控制传输的长???
	char buf[1000];
}train;
struct ls_m{
	char type;
	long size;
	char name[20];
	int flag;
};//传ls的信息给客户???
struct log{
	int len;
	char username[20];
	char salt[17];
	char cipher[177];
};//接收传输盐值和密文
struct regis{
	int len;
	char username[20];
	char salt[17];
	char *cipher;
};
void factory_init(pfac,pfunc,int);
void factory_start(pfac);
int send_n(int,char*,int);
int recv_n(int,char*,int);
void send_file(int,char*);
void recv_file(int,char*,char*);
int send_pwd(int,char*,char*);//发送路径函???
int cd_address(int,char*,char*);
int ls_address(int,char*);//ls处理函数
int send_remove(int,char*,char*);//remove处理函数
void mkdir_address(int sfd,char *,char*);
int login(int,char*);//处理登录
int regis(int);//处理注册

int ret_precode(char*,char*,char*);//返回precode值作为上传文件的precode值，传入路径
int findcode(char*,char*);//查找code值，作为上传文件的precode值，传入文件名或文件夹名
int file_exist(char*);//查找文件是否存在
void file_insert(int,char*,char *,char *,char *,char *);
	//插入文件操作
void find_abspath(char*,char*);//根据md5码去查找绝对路径
#endif












