#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <strings.h>
#include <sys/select.h>
#include <sys/time.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <signal.h>
#include <pthread.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/epoll.h>
#include <shadow.h>
#include <crypt.h>
#include <math.h>
#include <openssl/md5.h>
typedef struct{
	pid_t pid;
	int fd;//父子进程使用的全双工管道
	short busy;//1代表忙碌，0代表非忙碌
}Data,*pData;
typedef struct{
	int len;//控制数据
	char buf[1000];
}train;
typedef struct instrain//发给服务器端的指令
{
	int ins;//标志是哪条指令
	int len;//指令第二部分的长度
	char buf[77];//存储指令的第二部分 
//	char buf1[77];//给gets发那个1000用的第三部分
}instrain;
void make_child(pData p,int n);
void handle(int);
void send_fd(int,int,int);
void recv_fd(int,int*,int*);
int send_file(int,char*);
void recv_file(int,char*);//接收文件，在哪个路径接收文件
int send_n(int,char*,int);
int recv_n(int,char*,int);
int inscomfirm(char*);

int recv_cdresult(int);//接收执行cd指令后返回的文件路径并打印
int recv_lsresult(int);//接收执行ls指令后返回的结果
void recv_putsresult(int,char*);//上传文件
void recv_getsresult(int,char*);//下载文件
int recv_removeresult(int);//删除文件后的反馈
int recv_pwdresult(int);//查看当前路径
void recv_mkdir(int);//接收mkdir命令返回信息
void get_salt(char*);//生成盐值
int signin(int);//登录
int regis(int);//注册
off_t is_exist(char*);//判断文件是否存在并返回文件大小
int get_md5(char*,unsigned char*);//类型一定是unsigned char*



