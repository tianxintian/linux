#include"factory.h"
#define fileserver "localhost"
#define fileuser "root"
#define filepassword "123"
#define filedatabase "fileserver"
void get_salt(char *salt){
	char *str = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrssuvw    xyz,./;'<>    ?";
	int i,len=strlen(str);
	srand((unsigned int)time((time_t *)NULL));//使用系统时间来初始化随机数发
//	生器
		salt[0]='$';
	salt[1]='6';
	salt[2]='$';
	for(i=3;i<11;i++)
	{
		salt[i]=str[(rand()%len)];
	}
}


int login(int sfd,char *username)
{
	int ret;
	int count=0;//计算认证的次数
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char*server=fileserver;
	char*user=fileuser;
	char*password=filepassword;
	char*database=filedatabase;
	struct log r;//接收发来用户名
start:
	bzero(&r,sizeof(r));
	recv_n(sfd,(char*)&r,sizeof(r));
	char query[300]={0};//存储执行语句
	strcpy(query,"select * from user where username='");
	strcat(query,r.username);
	strcat(query,"'");
	puts(query);
	conn=mysql_init(NULL);
	if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
	{
		printf("Error connecting to database:%s\n",mysql_error(conn));
	}else
	{
		printf("Connected...\n");
	}
	int t;
	char salt[25];
	char cipher[177];//用于接收的密文
	char scipher[177];//用于密文和crypt生成后发来的密文相比较
	//相等就认证成功
	bzero(salt,sizeof(salt));//fayanzhi
	bzero(scipher,sizeof(scipher));
	bzero(cipher,sizeof(cipher));
	t=mysql_query(conn,query);//执行语句
	if(t)
	{
		printf("shibai\n");
	}
	else
	{
		res=mysql_use_result(conn);
		if(res)
		{
			if((row=mysql_fetch_row(res))!=NULL)
			{//30 为存在用户
				ret=30;
				send_n(sfd,(char*)&ret,sizeof(ret));
				printf("用户存在\n");
				printf("%8s\n",row[2]);
				strcpy(salt,row[2]);
				strcpy(scipher,row[3]);
				printf("salt is %s\n",salt);
				printf("scipher is %s\n",scipher);
				send_n(sfd,salt,sizeof(salt));
				recv_n(sfd,cipher,177);
				if(!strcmp(cipher,scipher))
				{
					ret=1;//success
					printf("认证成功\n");
					strcpy(username,row[1]);
					//在这里定位到user路径或者创建用户目录
					send_n(sfd,(char*)&ret,sizeof(ret));
				}else
				{
					ret=2;//fail
					printf("认证失败\n");
					count++;
					printf("count is %d\n",count);
					if(count==3)
					{
						send_n(sfd,(char*)&ret,sizeof(ret));
						goto flag1;
					}
					send_n(sfd,(char*)&ret,sizeof(ret));
					goto start;
				}
				goto flag1;
			}
			printf("用户不存在\n");
			ret=31;//31为不存在用户
			send_n(sfd,(char*)&ret,sizeof(ret));
			goto start;
		}
flag1:
		mysql_free_result(res);
	}
	mysql_close(conn);
	if(count==3)
	{
		return 33;
	}
	printf("%d\n",r.len);
	printf("%s\n",r.username);
	return 0;
}

int regis(int sfd)
{
	int ret;
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char* server=fileserver;
	char* user=fileuser;
	char* password=filepassword;
	char* database=filedatabase;
	char username[25];
	char salt[25];
	char ciphertext[177];
	bzero(username,sizeof(username));
	bzero(salt,sizeof(salt));
	bzero(ciphertext,sizeof(ciphertext));
	get_salt(salt);//服务器端生成盐值，发给客户端，客户端拿这个合成密文发来
	//服务器接收来自客户端的数据
	send_n(sfd,salt,sizeof(salt));
	recv_n(sfd,username,sizeof(username));//接收用户名
	printf("username is:%s\n",username);
	recv_n(sfd,ciphertext,177);//接收密文
	printf("cipher is %s\n",ciphertext);
	char query[200];//保存插入语句
	bzero(query,sizeof(query));
	sprintf(query,"insert into user(username,salt,ciphertext) values('%s','%s','%s')",username,salt,ciphertext);
	puts(query);
	ret=1;
	send_n(sfd,(char*)&ret,sizeof(ret));
	printf("daole zheyibulaaaa\n");
	int t;
	conn=mysql_init(NULL);
	if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
	{
		printf("Error connecting to database:%s\n",mysql_error(conn));
	}else{
		printf("Connected...\n");
	}
	t=mysql_query(conn,query);
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(conn));
	}else{
		printf("insert success\n");
	}
	mysql_close(conn);
	return 0;

}
