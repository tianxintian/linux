#include"func.h"
int main(int argc,char*argv[])
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
	int ret;
	ret=bind(sfd,(struct sockaddr*)&seraddr,sizeof(seraddr));
	if(ret==-1)
	{
		perror("bind");
		return -1;
	}
	fd_set rdset;
	struct sockaddr_in cliaddr;
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
				int len=sizeof(struct sockaddr);
	   			char buf[128]={0};
				ret=recvfrom(sfd,buf,sizeof(buf),0,(struct sockaddr*)&cliaddr,&len);
				if(ret==-1)
				{
					perror("recvfrom");
					break;
				}
			 	printf("%s",buf);
			}
			if(FD_ISSET(0,&rdset))
			{
				char buf1[128]={0};
				bzero(buf1,sizeof(buf1));
				ret=read(0,buf1,sizeof(buf1));
				if(ret<=0)
				{
					printf("readret=%d\n",ret);
					break;
				}
				int len1=strlen(buf1);
	 	        sendto(sfd,buf1,len1,0,(struct sockaddr*)&cliaddr,sizeof(struct sockaddr));
			}
		}
	}
	close(sfd);
	return 0;
}
