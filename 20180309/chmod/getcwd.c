#include <unistd.h>
#include <stdio.h>

int main()
{
	char path[512];
	char *p;
	//getcwd(path,sizeof(path));
	p=getcwd(NULL,0);
	puts(p);
	return 0;
}
