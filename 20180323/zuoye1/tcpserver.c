#include"func.h"
void epoll_add(int epfd, int fd)
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
int main(int argc, char* argv[])
{
	if(argc!=3)
	{
		printf("./server failed\n");
		return -1;
	}
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
	listen(sfd,10);
	int epfd=epoll_create(1);
	struct epoll_event evs[3];
	int new_fd;
	struct sockaddr_in cliaddr;
	int addrlen=sizeof(cliaddr);
	epoll_add(epfd,0);
	epoll_add(epfd,sfd);
	char buf[128]={0};
	int i,numret;
	while(1)
	{
		bzero(evs,sizeof(evs));
		numret=epoll_wait(epfd,evs,3,-1);
		for(i=0;i<numret;i++)
		{
			if(sfd==evs[i].data.fd)
			{
				bzero(&cliaddr,sizeof(cliaddr));
				new_fd=accept(sfd,(struct sockaddr*)&cliaddr,&addrlen);
				if(new_fd==-1)
				{
					perror("accept");
					return -1;
				}
				epoll_add(epfd,new_fd);
			}
			if(new_fd==evs[i].data.fd)
			{
				bzero(buf,sizeof(buf));
				ret=recv(new_fd,buf,sizeof(buf),0);
				if(ret==-1)
				{
					perror("recv");
					return -1;
				}else if(ret==0)
				{
					printf("byebye\n");
					epoll_del(epfd,new_fd);
					close(new_fd);
					break;
				}
				printf("%s\n",buf);
			}
			if(evs[i].events==EPOLLIN&&evs[i].data.fd==0)
			{
				bzero(buf,sizeof(buf));
				ret=read(STDIN_FILENO,buf,sizeof(buf));
				if(ret==0)
				{
					printf("byebye\n");
					epoll_del(epfd,new_fd);
					close(new_fd);
					break;
				}
				send(new_fd,buf,strlen(buf)-1,0);
			}
		}
	}
	close(sfd);
	return 0;
}

