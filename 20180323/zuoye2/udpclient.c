#include"func.h"
int main(int argc,char*argv[1])
{
	if(argc!=3)
	{
		printf("./server failed\n");
		return -1;
	}
	int sfd=socket(AF_INET,SOCK_DGRAM,0);
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
	fd_set rdset;
	int ret;
	struct sockaddr_in cliaddr;
	while(1)
	{
		FD_ZERO(&rdset);
		FD_SET(STDIN_FILENO,&rdset);
		FD_SET(sfd,&rdset);
		ret=select(sfd+1,&rdset,NULL,NULL,NULL);
		if(ret>0)
		{
			if(FD_ISSET(0,&rdset))
			{
				char buf[128]={0};
				bzero(buf,sizeof(buf));
				ret=read(0,buf,sizeof(buf));
				int len=strlen(buf);
				sendto(sfd,buf,len,0,(struct sockaddr*)&seraddr,sizeof(struct sockaddr));
			}
			if(FD_ISSET(sfd,&rdset))
			{
				int len1=sizeof(struct sockaddr);
				char buf1[128]={0};
				ret=recvfrom(sfd,buf1,sizeof(buf1),0,(struct sockaddr*)&cliaddr,&len1);
				if(ret==-1)
				{
					perror("recvfrom");
					break;
				}
				printf("%s",buf1);
			}
		}
	}
	close(sfd);
	return 0;
}
