#include"func.h"

void sigfunc(int sig)
{
	printf("before sleep %d\n",sig);
	sleep(3);
	printf("after sleep %d\n",sig);
}
int main()
{
	signal(SIGINT,sigfunc);
	signal(SIGQUIT,sigfunc);
	while(1);
}
