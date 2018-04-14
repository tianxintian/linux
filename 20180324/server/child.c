#include"func.h"
//创建子进程，父进程来控制用哪一个空闲的子进程执行任务
//fork子进程返回零，父进程返回子进程的pid
//pipe创建的描述符一端只能用于读，一端用于写，而socketpair创建的描述符任意一端既可以读也可以写。
void make_child(pData p,int n)
{
	int i;
	pid_t pid;
	int fds[2];
	for(i=0;i<n;i++)
	{
		socketpair(AF_LOCAL,SOCK_STREAM,0,fds);
		pid=fork();
		if(!pid)
		{
			close(fds[0]);//关闭读
			child_handle(fds[1]);//处理写
		}
		close(fds[1]);//关闭写
		p[i].pid=pid;//子进程的pid给结构体存储着
		p[i].fd=fds[0];//父进程对应读端赋值，每一个子进程写端对应的读端
		printf("pid=%d,fds[0]=%d\n",pid,fds[0]);
	}
}
void child_handle(int fd)
{
	int sfd;
	char unbusy='0';
	while(1)
	{
		recv_fd(fd,&sfd);//接受父亲程分配的任务，fds[1]标志是哪一个子进程
		printf("i will send file to customer %d\n",sfd);//任务是发送东西给客户
		tran_file(sfd);//发送文件给客户
		write(fd,&unbusy,sizeof(char));//通过写端写入管道表示不忙，父进程接收
	}
}
