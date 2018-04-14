#include <unistd.h>
#include <stdio.h>

int main(int argc,char* argv[])
{
	if(argc!=2)
	{
		printf("error args\n");
		return -1;
	}
	char path[512];
	char *p;
	//getcwd(path,sizeof(path));
	p=getcwd(NULL,0);
	puts(p);
	chdir(argv[1]);
	p=getcwd(NULL,0);
	puts(p);
	return 0;
}
