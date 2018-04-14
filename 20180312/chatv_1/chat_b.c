#include"func.h"
int main(int argc,char *argv[])
{
	if(argc!=2)
	{
		printf("error args.\n");
		return -1;
	}
	int fd;
	fd = open(argv[1],O_RDONLY);
	if(fd==-1)
	{
		perror("open");
		return -1;
	}
	printf("I am reader,fd=%d\n",fd);
	char buf[128]={0};
	int ret=read(fd,buf,sizeof(buf));
	if(ret==-1)
	{
		perror("read");
		return 0;
	}
	printf("i am reader,ret=%d,%s\n",ret,buf);
	return 0;
}
