#include"func.h"
#define filename "/tmp/log"
int main()
{
	if(fork())//父进程退出
	{
		exit(0);
	}
	setsid();//创建新会话
	chdir("/");//改变当前路径到根目录
	umask(0);//改变掩码
	int i;
	for(i=0;i<3;i++)
	{
		close(i);
	}
	int fp=open(filename,O_RDWR);
	while(1)
	{
		char buf[128]="i have sleep five seconds.\n";
    	int fdw=write(fp,buf,strlen(buf));
		if(fdw==-1)
		{
			perror("fail.");
			exit(-1);
		}
		sleep(5);
	}
	exit(0);
}
