#include"func.h"
int main(int argc,char *argv[])
{
	if(argc!=2)
	{
		printf("error args.\n");
		return -1;
	}
	int fd;
	fd = open(argv[1],O_WRONLY);
	if(fd == -1)
	{
		perror("open");
		return -1;
	}
	printf("I am writer,fd=%d\n",fd);
	write(fd,"hello",5);
	return 0;
}
