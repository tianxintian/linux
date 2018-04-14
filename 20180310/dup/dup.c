#include "func.h"

int main(int argc,char* argv[])
{
	if(argc!=2)
	{
		printf("error args\n");
		return -1;
	}
	int fd;
	fd=open(argv[1],O_RDWR);
	if(-1==fd)
	{
		perror("open");
		return -1;
	}
	int fd1;
	fd1=dup(fd);//描述符复制
	printf("fd1=%d\n",fd1);
	close(fd);
	char buf[128]="hello";
	int ret=write(fd1,buf,strlen(buf));
	printf("ret=%d\n",ret);
	return 0;
}
