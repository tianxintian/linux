#include<stdio.h>
int MyFunc();
int main()
{
#ifdef _DEBUG
	printf("Debug MyFunc is:%d\n",MyFunc());
#else
	printf("NDEBUG MyFunc is:%d\n",MyFunc());;
#endif
}
