#include"func.h"

struct log{//登录的时候存储发送给服务器的数据
	int len;
	char username[20];
	char salt[17];
	char cipher[177];
};
/*struct regis{//注册的时候发送给服务器端的数据
  int len;
  char username[20];
  char salt[17];
  char *cipher;
  };*/
int signin(int sfd)//登录函数
{
	int i;
	int count=0;
	int ret;
	char *str="sancimimacuowu";
	char username[25];
	char *pwd=NULL;
	char salt[25];
	char *ciphertext=NULL;
	while(1)
	{
start:
		bzero(username,sizeof(username));
		bzero(salt,sizeof(salt));
		fflush(stdout);
		printf("请输入用户名:");
		fgets(username,sizeof(username),stdin);
		username[strlen(username)-1]=0;
		struct log s;//把用户名发送过去
		bzero(&s,sizeof(s));
		strcpy(s.username,username);
		send_n(sfd,(char*)&s,sizeof(s));
		recv_n(sfd,(char*)&ret,sizeof(ret));//有无用户
		if(ret==31)
		{
			goto start;
		}
		printf("len is %d username is %s\n",s.len,s.username);
		recv_n(sfd,salt,sizeof(salt));
		printf("yanzhi is %s\n",salt);
		printf("%s\n",username);
		pwd=getpass("请输入密码:");
		ciphertext=crypt(pwd,salt);
		send_n(sfd,ciphertext,177);//把生成的密文发送给服务器比对
		recv_n(sfd,(char*)&ret,sizeof(ret));
		if(ret==1)//输入密码正确，登录成功
		{
			printf("认证成功\n");
			goto end;		
		}
		if(ret==2)//有用户但是三次输密码错误
		{
			count++;
			printf("count is %d\n",count);
			if(count==3)
			{
				printf("三次密码错误\n");
				return 33;//表示不再接受登录了，重新选择1或者2.
			}
			printf("try again\n");
			goto start;
		}
		printf("%s\n",pwd);
		printf("%s\n",ciphertext);
	}

end:
	return 0;
}

int regis(int sfd)
{
	int ret;
	char username[25];
	char *pwd=NULL;
	char salt[25];
	char cipher[177];
	char *ciphertext=NULL;
	bzero(username,sizeof(username));
	bzero(salt,sizeof(salt));
	bzero(cipher,sizeof(cipher));
	recv_n(sfd,salt,sizeof(salt));//接收服务器发来的盐值
	printf("salt is %s\n",salt);
	fflush(stdout);
	printf("请输入用户名:");
	fgets(username,sizeof(username),stdin);
	username[strlen(username)-1]=0;
	send_n(sfd,username,sizeof(username));//发给服务器用户名
	pwd=getpass("请输入密码:");
	ciphertext=crypt(pwd,salt);
	printf("cipher is %s\n",ciphertext);
	strcpy(cipher,ciphertext);
	send_n(sfd,cipher,177);
	//发给服务器密文
	//	printf("%s\n%s\n%s\n",s.username,s.salt,s.cipher);
	//	printf("wodaolalalala\n");
	recv_n(sfd,(char*)&ret,sizeof(ret));
	if(ret==1)
	{
		printf("insert success\n");
	}
	return 0;
}














