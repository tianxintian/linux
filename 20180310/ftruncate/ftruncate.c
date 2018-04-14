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
	int ret=ftruncate(fd,3);
	if(-1==ret)
	{
		perror("ftruncate");
		return -1;
	}
	return 0;
}
