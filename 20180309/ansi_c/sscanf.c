#include <stdio.h>
#include <string.h>
int main()
{
	int i=10;
	float f=98.5;
	char name[20]="xiongda";
	char buf[128]={0};
	sprintf(buf,"%s %d %5.2f",name,i,f);
	puts(buf);
	printf("----------------------\n");
	i=0;
	f=0;
	memset(name,0,sizeof(name));
	sscanf(buf,"%s%d%f",name,&i,&f);
	printf("i=%d,f=%5.2f,name=%s\n",i,f,name);
	return 0;
}
