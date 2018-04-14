#include"func.h"//执行多点下载，可以通过epoll监控链接的sfd然后分配新的new_fd让他们
void epoll_add(int epfd,int fd)//分别去执行下载任务
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
		printf("./server ip port process_num\n");
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
	int ret1;
	while(1)
	{
		bzero(evs,sizeof(struct epoll_event)*(num+1));
		ret=epoll_wait(epfd,evs,num+1,-1);
		for(i=0;i<ret;i++)//寻找到建立链接的sfd起作用了，就执行新建new_fd
		{
			if(sfd==evs[i].data.fd)
			{
				new_fd=accept(sfd,NULL,NULL);
				for(j=0;j<num;j++)//找一个不忙的子进程分配任务给他
				{
					if(p[j].busy==0)
					{
						break;
					}
				}
				send_fd(p[j].fd,new_fd);//让子进程拿着new_fd传入msg结构体，p[j].fd标志哪一个子进程的作用，毕竟这是一个数字，是一个描述符。
				close(new_fd);//子进程在对端接收到了msg结构体，用sfd接受，接着子进程继续
				p[j].busy=1;//置子进程忙
			}
			for(j=0;j<num;j++)
			{
				if(p[j].fd==evs[i].data.fd)//就是子进程执行的fds[0]读端成立，fds[1]向管道写入了数据，这时候就成立了。
				{
					ret1=read(p[j].fd,&flag,sizeof(char));//读取到flag里面，表示不忙了
					p[j].busy=0;//置为不忙
				}
			}
		}
	}
}

