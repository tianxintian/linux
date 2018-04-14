#include "func.h"
//实现mmap
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
	char *p;
	p=(char*)mmap(NULL,4096,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	if((char*)-1==p)
	{
		perror("mmap");
		return -1;
	}
	strcpy(p,"HELLO");
	int ret=munmap(p,4096);
	if(-1==ret)
	{
		perror("munmap");
		return -1;
	}
	return 0;
}
