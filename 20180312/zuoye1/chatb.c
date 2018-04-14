#include"func.h"
int main(int argc,char*argv[])
{
	if(argc!=3)
	{
		printf("error args\n");
		return -1;
	}
	int fdr,fdw;
	fdw=open(argv[1],O_WRONLY);
	if(fdw==-1)
	{
		perror("open1");
		return -1;
	}
	fdr=open(argv[2],O_RDONLY);
	if(fdr==-1)
	{
		perror("open");
		return -1;
	}
	char buf[129]={0};
	int ret;
	fd_set rdset;
	while(1)
	{
		FD_ZERO(&rdset);
		FD_SET(0,&rdset);
		FD_SET(fdr,&rdset);
		ret=select(fdr+1,&rdset,NULL,NULL,NULL);
		if(ret>0)
		{
			if(FD_ISSET(0,&rdset))
			{
				bzero(buf,sizeof(buf));
				ret=read(0,buf,sizeof(buf)-1);
				if(ret==0)
				{
					printf("end read\n");
					break;
				}
				write(fdw,buf,strlen(buf)-1);
			}
			if(FD_ISSET(fdr,&rdset))
			{
				bzero(buf,sizeof(buf));
				ret=read(fdr,buf,sizeof(buf));
				if(ret==0)
				{
					printf("byebye\n");
					break;
				}
				printf("%s\n",buf);
			}
		}
	}
	close(fdw);
	close(fdr);
	return 0;
}










































