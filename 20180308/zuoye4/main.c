#include<stdio.h>
#include<string.h>
void print(int j)
{
	j = j+3;
	printf("j=%d\n",j);
}
#define N 1+2
typedef int INTEGER;
int main(int argc,char **argv)
{
	INTEGER i;
	for(i = 0; i < argc;i++)
	{
		puts(argv[i]);
	}
	int k;
	k = 10;
	i = N * N;
	print(i);
	return 0;
}
