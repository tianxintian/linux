#include"func.h"
void sendfile(int new_fd)
{
	signal(SIGPIPE,SIG_IGN);
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
	struct stat buf;
	int ret=fstat(fd,&buf);
	if(ret==-1)
	{
		perror("fstat");
		goto end;
	}
	d.len=sizeof(off_t);
	memcpy(d.buf,&buf.st_size,d.len);
	send_n(new_fd,(char*)&d,4+d.len);
	while((d.len=read(fd,d.buf,sizeof(d.buf)))>0)
	{
		ret=send_n(new_fd,(char*)&d,4+d.len);
		if(ret==-1)
		{
			goto end;
		}
	}
	d.len=0;
	send_n(new_fd,(char*)&d,4);
end:
	close(new_fd);
}
