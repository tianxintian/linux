#include"func.h"
int main()
{
	pid_t pid,ppid;
	pid=getpid();
	ppid=getppid();
	printf("pid=%d,ppid=%d\n",pid,ppid);
	int fd;
	fd=open("file",O_RDWR);
	if(fd==-1)
	{
		perror("open");
		return -1;
	}
	write(fd,"hello",5);
	close(fd);
	return 0;
}
