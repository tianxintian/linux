#include "func.h"

int main(int argc,char *argv[])
{
	if(argc!=2)
	{
		printf("error args\n");
		return -1;
	}
	struct stat buf;
	int ret;
	ret=stat(argv[1],&buf);
	if(-1==ret)
	{
		perror("stat");
		return -1;
	}
	printf("%ld\n",sizeof(mode_t));
	printf("ino=%ld,mode=%x,nlink=%ld,uid=%s,gid=%s,size=%ld,mtime=%s\n",buf.st_ino,buf.st_mode,buf.st_nlink,getpwuid(buf.st_uid)->pw_name,getgrgid(buf.st_gid)->gr_name,buf.st_size,ctime(&buf.st_mtime)+4);
	return 0;
}
