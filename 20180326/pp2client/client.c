#include"func.h"
int main(int argc,char *argv[])
{
	if(argc!=3)
	{
		printf("./server ip port\n");
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
	int len;
	char buf[1000]={0};
	ret=recv(sfd,&len,sizeof(len),0);
	if(ret==0)
	{
		printf("server close\n");
		goto end;
	}
	ret=recv(sfd,buf,len,0);
	if(ret==0)
	{
		printf("server close\n");
		goto end;
	}
	int fd;
	fd=open(buf,O_RDWR|O_CREAT,0666);
	off_t f_size;
	ret=recv_n(sfd,(char*)&len,sizeof(len));
	if(ret==-1)
	{
		printf("server close\n");
		goto end;
	}
	ret=recv_n(sfd,(char*)&f_size,len);
	if(ret==-1)
	{
		printf("server close\n");
		goto end;
	}
	float f=0;
	int j=1;
	time_t now,last;
	now=time(NULL);
	last=now;
	printf("loaded:\n");
	while(1)
	{
		ret=recv_n(sfd,(char*)&len,sizeof(len));
		if(ret==-1)
		{
			printf("%5.2f%s\n",f/f_size*100,"%");
			break;
		}
		else if(len>0)
		{
			f=f+len;
			time(&now);
			if(now-last>=1)
			{
				printf("%5.2f%s\r",f/f_size*100,"%");
				fflush(stdout);
				last=now;
			}
			ret=recv_n(sfd,buf,len);
			write(fd,buf,len);
			if(ret==-1)
			{
				printf("%5.2f%s\n",f/f_size*100,"%");
				break;
			}
		}else
		{
			printf("       \r");
			printf("%d%s\n",100,"%");
			break;
		}
	}
end:
	close(fd);
	close(sfd);
}
