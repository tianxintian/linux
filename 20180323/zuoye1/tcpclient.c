#include"func.h"
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
	struct sockaddr_in seraddr;
	bzero(&seraddr,sizeof(seraddr));
	seraddr.sin_family=AF_INET;
	seraddr.sin_port=htons(atoi(argv[2]));
	seraddr.sin_addr.s_addr=inet_addr(argv[1]);
	int ret;
	ret=connect(sfd,(struct sockaddr*)&seraddr,sizeof(seraddr));
	if(ret==-1)
	{
		perror("connect");
		return -1;
	}
	char buf[128]={0};
	fd_set rdset;
	while(1)
	{
		FD_ZERO(&rdset);
		FD_SET(STDIN_FILENO,&rdset);
		FD_SET(sfd,&rdset);
		ret=select(sfd+1,&rdset,NULL,NULL,NULL);
		if(ret>0)
		{
			if(FD_ISSET(sfd,&rdset))
			{
				bzero(buf,sizeof(buf));
				ret=recv(sfd,buf,sizeof(buf),0);
				if(ret==-1)
				{
					perror("recv");
					return -1;
				}else if(ret==0)
				{
					printf("byebye\n");
					break;
				}
				printf("%s\n",buf);
			}
			if(FD_ISSET(0,&rdset))
			{
				bzero(buf,sizeof(buf));
				ret=read(STDIN_FILENO,buf,sizeof(buf));
				if(ret<=0)
				{
					printf("read ret=%d\n",ret);
					break;
				}
				send(sfd,buf,strlen(buf)-1,0);
			}
		}
	}
	close(sfd);
	return 0;
}
