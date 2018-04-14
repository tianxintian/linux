#include "func.h"
//实现重定向
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
	printf("\n");
	close(1);
	int fd1;
	fd1=dup(fd);//描述符复制
	close(fd);
	printf("you can't see me\n");
	return 0;
}
