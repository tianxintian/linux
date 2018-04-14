#include"func.h"
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
			close(fds[0]);
			handle(fds[1]);
		}
		close(fds[1]);
		p[i].pid=pid;
		p[i].fd=fds[0];
		printf("pid=%d,fds[0] is %d\n",pid,fds[0]);
	}
}
void handle(int fd)
{
	int sfd;
	char unbusy='0';
	int exit_flag;
	while(1)
	{
		recv_fd(fd,&sfd,&exit_flag);
		if(exit_flag)
		{
			printf("i am child,i will exit\n");
			exit(0);
		}
		printf("i will send file to customer %d\n",sfd);
		sendfile(sfd);
		write(fd,&unbusy,sizeof(char));
	}
}
