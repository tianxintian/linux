#include"func.h"
int main(int argc,char* argv[])
{
	if(argc!=3)
	{
		printf("server ip failed\n");
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
	int ret=connect(sfd,(struct sockaddr*)&seraddr,sizeof(seraddr));
	if(ret==-1)
	{
		perror("connect failed\n");
		return -1;
	}
	printf("client\n");
	send(sfd,"okokokfuwuqi",12,0);
	char buf[128]={0};
	recv(sfd,buf,sizeof(buf),0);
	printf("%s\n",buf);
	close(sfd);
	return 0;
}
