#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>
int main(int argc,char* argv[])
{
	if(argc!=2)
	{
		printf("error args\n");
		return -1;
	}
	umask(0);
	int ret;
	ret=mkdir(argv[1],0777);
	if(-1==ret)
	{
		printf("errno=%d\n",errno);
		perror("mkdir");
		return -1;
	}
	return 0;
}
