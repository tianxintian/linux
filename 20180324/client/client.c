#include"func.h"
int recv_n(int sfd,char* buf,int len)
{
	int total=0;
	int ret;

	while(total<len)
	{
		ret=recv(sfd,buf+total,len-total,0);
		total=total+ret;
	}
	return 0;
}		               
int main(int argc,char*argv[])
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
	struct sockaddr_in ser;
	bzero(&ser,sizeof(ser));
	ser.sin_family=AF_INET;
	ser.sin_port=htons(atoi(argv[2]));
	ser.sin_addr.s_addr=inet_addr(argv[1]);
	int ret;
	ret=connect(sfd,(struct sockaddr*)&ser,sizeof(ser));
	if(ret==-1)
	{
		perror("connect");
		return -1;
	}
	int len;
	char buf[1000]={0};
	recv(sfd,&len,sizeof(len),0);
	recv(sfd,buf,len,0);
	int fd;
	fd=open(buf,O_RDWR|O_CREAT,0666);
	while(1)
	{
		recv_n(sfd,(char*)&len,sizeof(len));
		if(len>0)
		{
			recv_n(sfd,buf,len);
			write(fd,buf,len);
		}else
		{
			break;
		}
	}
	close(fd);
	close(sfd);
}
