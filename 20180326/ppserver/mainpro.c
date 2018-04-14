#include"func.h"
void epoll_add(int epfd,int fd)
{
	struct epoll_event event;
	bzero(&event,sizeof(event));
	event.events=EPOLLIN;
	event.data.fd=fd;
	int ret=epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&event);
	if(ret==-1)
	{
		perror("epoll_ctl");
		return;
	}
}
void epoll_del(int epfd,int fd)
{
	struct epoll_event event;
	bzero(&event,sizeof(event));
	event.events=EPOLLIN;
	event.data.fd=fd;
	int ret=epoll_ctl(epfd,EPOLL_CTL_DEL,fd,&event);
	if(ret==-1)
	{
		perror("epoll_ctl");
		return;
	}
}
int fds[2];
void sigfunc(int sig)
{
	char flag='g';
	write(fds[1],&flag,sizeof(flag));
}
int main(int argc,char* argv[])
{
	if(argc!=4)
	{
		printf("./server ip port pnum\n");
		return -1;
	}
	pipe(fds);
	signal(SIGUSR1,sigfunc);
	int num=atoi(argv[3]);
	pData p=(pData)calloc(num,sizeof(Data));
	make_child(p,num);
	int sfd=socket(AF_INET,SOCK_STREAM,0);
	if(sfd==-1)
	{
		perror("socket");
		return -1;
	}
	int reuse=1;
	int ret;
	ret=setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(int));
	if(ret==-1)
	{
		perror("setsocketopt");
		return -1;
	}
	struct sockaddr_in seraddr;
	bzero(&seraddr,sizeof(seraddr));
	seraddr.sin_family=AF_INET;
	seraddr.sin_port=htons(atoi(argv[2]));
	seraddr.sin_addr.s_addr=inet_addr(argv[1]);
	ret=bind(sfd,(struct sockaddr*)&seraddr,sizeof(seraddr));
	if(ret==-1)
	{
		perror("bind");
		return -1;
	}
	int epfd=epoll_create(1);
	epoll_add(epfd,sfd);
	int i,j;
	for(i=0;i<num;i++)
	{
		epoll_add(epfd,p[i].fd);
	}	
	listen(sfd,10);
	struct epoll_event*evs=(struct epoll_event*)calloc(num+2,sizeof(struct epoll_event));
	int new_fd;
	char flag;
	int ret1;
	epoll_add(epfd,fds[0]);
	while(1)
	{
		bzero(evs,sizeof(struct epoll_event)*(num+2));
		ret=epoll_wait(epfd,evs,num+2,-1);
		for(i=0;i<ret;i++)
		{
			if(sfd==evs[i].data.fd)
			{
				new_fd=accept(sfd,NULL,NULL);
				for(j=0;j<num;j++)
				{
					if(p[j].busy==0)
					{
						send_fd(p[j].fd,new_fd,0);
						break;
					}
				}
				close(new_fd);
				p[j].busy=1;
				printf("p[j].pid=%d is sending file.\n",p[j].pid);
			}
			if(fds[0]==evs[i].data.fd)
			{
				epoll_del(epfd,sfd);
				close(sfd);
				for(j=0;j<num;j++)
				{
					send_fd(p[j].fd,0,1);
				}
				for(j=0;j<num;j++)
				{
					wait(NULL);
				}
				exit(0);
			}
			for(j=0;j<num;j++)
			{
				if(p[j].fd==evs[i].data.fd)
				{
					ret1=read(p[j].fd,&flag,sizeof(char));
					printf("p[j].pid=%d is not busy.",p[j].pid);
					p[j].busy=0;
				}
			}
		}
	}
}

