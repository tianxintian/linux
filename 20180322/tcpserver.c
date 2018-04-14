#include"func.h"
int main(int argc,char *argv[])
{
	if(argc!=3)
	{
		printf("server failed\n");
		return -1;
	}
	int sfd=socket(AF_INET,SOCK_STREAM,0);
	if(sfd==-1)
	{
		perror("socket failed\n");
		return -1;
	}
	struct sockaddr_in seraddr;
	bzero(&seraddr,sizeof(seraddr));
	seraddr.sin_family=AF_INET;
	seraddr.sin_port=htons(atoi(argv[2]));
	seraddr.sin_addr.s_addr=inet_addr(argv[1]);
	int ret;
	bind(sfd,(struct sockaddr*)&seraddr,sizeof(seraddr));
	if(ret==-1)
	{
		perror("bind failed");
		return -1;
	}
	listen(sfd,10);//激活
	struct sockaddr_in cliaddr;
	bzero(&cliaddr,sizeof(cliaddr));
	int addrlen=sizeof(cliaddr);
	int new_fd=accept(sfd,(struct sockaddr*)&cliaddr,&addrlen);
	if(new_fd==-1)
	{
		perror("accept failed\n");
		return -1;
	}
	printf("cliaddr ip=%s,cliaddr port=%d\n",inet_ntoa(cliaddr.sin_addr),ntohs(cliaddr.sin_port));
	printf("new_fd=%d\n",new_fd);
	char buf[128]={0};
	ret=recv(new_fd,buf,sizeof(buf),0);
	if(ret==-1)
	{
		perror("recv failed\n");
		return -1;
	}
	printf("server ,recv=%s\n",buf);
	send(new_fd,"okokokkehu",10,0);
	close(new_fd);
	close(sfd);
	return 0;
}










