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
int main(int argc,char* argv[])
{
	if(argc!=4)
	{
		printf("./server failed\n");
		return -1;
	}
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
		perror("setsockopt");
		return -1;
	}
	struct sockaddr_in ser;
	bzero(&ser,sizeof(ser));
	ser.sin_family=AF_INET;
	ser.sin_port=htons(atoi(argv[2]));
	ser.sin_addr.s_addr=inet_addr(argv[1]);
	ret=bind(sfd,(struct sockaddr*)&ser,sizeof(ser));
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
	struct epoll_event *evs=(struct epoll_event*)calloc(num+1,sizeof(struct epoll_event));
	int new_fd;
	char flag;
	while(1)
	{
		bzero(evs,sizeof(struct epoll_event)*(num+1));
		ret=epoll_wait(epfd,evs,num+1,-1);
		for(i=0;i<ret;i++)
		{
			if(sfd==evs[i].data.fd)
			{
				new_fd=accept(sfd,NULL,NULL);
				for(j=0;j<num;j++)
				{
					if(p[j].busy==0)
					{
						break;
					}
				}
				send_fd(p[j].fd,new_fd);
				close(new_fd);
				p[j].busy=1;
				printf("p[j].pid=%d is busy\n",p[j].pid);
			}
			for(j=0;j<num;j++)
			{
				if(p[j].fd==evs[i].data.fd)
				{
					read(p[j].fd,&flag,sizeof(char));
					printf("p[j].pid=%d is not busy\n",p[j].pid);
					p[j].busy=0;
				}
			}
		}
	}
}
