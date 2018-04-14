#include"func.h"
typedef struct student{
	char num[10];
	char name[10];
	char score[10];
}stu;
int main(int argc,char*argv[])
{
	stu s[3]={{"1001","xiongda","99.9"},{"1002","xionger","98.9"},{"1003","xiongbb","99.8"}};
	char space[2]=" ";
	char hang[2]="\n";
	int i;
	int fd;
	int count = 3;
	fd=open(argv[1],O_RDWR);
	if(fd==-1)
	{
		perror("open");
		return -1;
	}
	for(i=0;i<count;i++)
	{
		write(fd,s[i].num,strlen(s[i].num));
		write(fd,space,1);
		write(fd,s[i].name,strlen(s[i].name));
		write(fd,space,1);
		write(fd,s[i].score,strlen(s[i].score));
		write(fd,hang,1);
	}
	lseek(fd,0,SEEK_SET);
	memset(s,0,sizeof(stu)*3);
	char buf[150]={0};
	int ret;
	while((ret=read(fd,buf,sizeof(buf)))>0)
	{
		printf("%s",buf);
	}
	close(fd);
	return 0;
}











