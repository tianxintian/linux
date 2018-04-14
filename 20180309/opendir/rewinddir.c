#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
int main(int argc,char *argv[])
{
	if(argc!=2)
	{
		printf("error args\n");
		return -1;
	}
	DIR *dir;//目录指针
	dir=opendir(argv[1]);
	if(NULL==dir)
	{
		perror("opendir");
		return -1;
	}
	struct dirent *p;
	off_t pos;
	while((p=readdir(dir))!=NULL)
	{
		printf("d_ino=%ld,d_off=%ld,d_reclen=%d,d_type=%d,d_name=%s\n",p->d_ino,p->d_off,p->d_reclen,p->d_type,p->d_name);
	}
	printf("---------------\n");
	rewinddir(dir);
	p=readdir(dir);
	printf("d_ino=%ld,d_off=%ld,d_reclen=%d,d_type=%d,d_name=%s\n",p->d_ino,p->d_off,p->d_reclen,p->d_type,p->d_name);
	closedir(dir);
	return 0;
}
