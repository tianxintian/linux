#include"func.h"
int main()
{
	int fds[2];
	pipe(fds);
	if(!fork())
	{
		close(fds[1]);
		printf("erzi\n");
		char buf[122]={0};
		read(fds[0],buf,sizeof(buf));
		printf("i am erzi.%s\n",buf);
		close(fds[0]);
	}else
	{
		close(fds[0]);
		printf("parent.\n");
		write(fds[1],"hello",5);
		wait(NULL);
		close(fds[1]);
	}
	exit(0);
}
