#include "func.h"
//制造文件空洞
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
	int ret;
	ret=lseek(fd,1024,SEEK_SET);
	if(-1==ret)
	{
		perror("lseek");
		return -1;
	}
	printf("pos=%d\n",ret);
	char c='h';
	write(fd,&c,sizeof(c));
	return 0;
}
