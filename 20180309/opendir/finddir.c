#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
//深度优先遍历目录文件
void printdir(char *path,int width)
{
	DIR *dir;//目录指针
	dir=opendir(path);
	if(NULL==dir)
	{
		perror("opendir");
		return;
	}
	struct dirent *p;
	char buf[512]={0};
	while((p=readdir(dir))!=NULL)
	{
		if(!strcmp(p->d_name,".")||!strcmp(p->d_name,".."))//去除.和..目录
		{
		}else{			
			printf("%*s%s\n",width,"",p->d_name);//打印文件或者目录名字
			if(4==p->d_type)
			{
				sprintf(buf,"%s%s%s",path,"/",p->d_name);//路径拼接
				printdir(buf,width+4);
			}
		}
	}
	closedir(dir);
}
int main(int argc,char *argv[])
{
	if(argc!=2)
	{
		printf("error args\n");
		return -1;
	}
	puts(argv[1]);
	printdir(argv[1],0);
	return 0;
}
