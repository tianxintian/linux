#include"func.h"
void sigfunc(int sig,siginfo_t *p,void *p1)
{
	printf("before %d\n",sig);
	sleep(3);
	sigset_t set;
	sigemptyset(&set);
	sigpending(&set);
	if(sigismember(&set,SIGQUIT))
	{
		printf("SIGQUIT is pending\n");
	}else
	{
		printf("SIGQUIT is not pending\n");
	}
	printf("after %d\n",sig);
}
int main()
{
	struct sigaction act;
	memset(&act,0,sizeof(act));
	act.sa_sigaction=sigfunc;
	act.sa_flags=SA_SIGINFO;
	sigemptyset(&act.sa_mask);
	sigaddset(&act.sa_mask,SIGQUIT);
	sigaction(SIGINT,&act,NULL);
	sigaction(SIGQUIT,&act,NULL);
	while(1);
	return 0;
}





















