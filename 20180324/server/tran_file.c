#include"func.h"
void sigfunc(int sig)
{
	printf("%d is coming\n");
}
void tran_file(int new_fd)
{
	signal(SIGPIPE,sigfunc);
	train d;
	strcpy(d.buf,FILENAME);
	d.len=strlen(d.buf);
	send(new_fd,&d,4+d.len,0);
	int fd;
	fd=open(FILENAME,O_RDONLY);
	if(fd==-1)
	{
		perror("open");
		goto end;
	}
	while((d.len=read(fd,d.buf,sizeof(d.buf)))>0)
	{
		send_n(new_fd,(char*)&d,4+d.len);
	}
	d.len=0;//实际上也可以不判断，直接返回0了
	send_n(new_fd,(char*)&d,4);
end:
	close(new_fd);
}
