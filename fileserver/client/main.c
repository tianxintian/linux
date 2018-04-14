//查看日志是执行两个命令
//cd /var/log  
//cat syslog
//再执行cd -命令回到起点
#include"func.h"

int main(int argc,char *argv[])
{
	if(argc!=3)
	{
		printf("./server ip port\n");
		return -1;
	}
	int sfd=socket(AF_INET,SOCK_STREAM,0);
	if(sfd==-1)
	{
		perror("socket");
		return -1;
	}
	struct sockaddr_in seraddr;
	bzero(&seraddr,sizeof(seraddr));
	seraddr.sin_family=AF_INET;
	seraddr.sin_port=htons(atoi(argv[2]));
	seraddr.sin_addr.s_addr=inet_addr(argv[1]);
	int ret;
	ret=connect(sfd,(struct sockaddr*)&seraddr,sizeof(seraddr));
	if(ret==-1)
	{
		perror("connect");
		return -1;
	}
flag2:
	//当三次输入密码错误，认证失败的时候回到这里选择
	printf("1-signin___2-register\n");
	int xuanze;
	int ret2;
	//bzero(sanfail,sizeof(sanfail));
	char ch[3];
	fflush(stdout);
	bzero(ch,sizeof(ch));
	read(0,ch,sizeof(ch));
	xuanze=atoi(ch);
	switch(xuanze)
	{
		case 1:
			send_n(sfd,(char*)&xuanze,sizeof(xuanze));
denglu:
			ret2=signin(sfd);
			if(ret2==33)//用户名是对的，密码错了三次，跳到开始再进行选择
			{
				goto flag2;
			}
			break;
		case 2:
			send_n(sfd,(char*)&xuanze,sizeof(xuanze));
			ret2=regis(sfd);//注册函数
			goto denglu;
			break;
	}
//	ret=sign_regi(sfd);*/
	char path[177]={0};//接收服务器发来的路径，定位到相应路径操作
	while(1)
	{
	//	signal(SIGINT,SIG_IGN);
		struct instrain insmes;
start:
		bzero(&insmes,sizeof(insmes));
		char insin[77]={0};//输入的指令
		printf("enter your instruction:");
		fgets(insin,77,stdin);
		int len=strlen(insin);
		int ret;//接收返回值
		char mymd5[177]={0};
	    char *insone,*instwo;
		int i=0;
		for(i=0;i<len;i++)
		{
			if(insin[i]==' ')
			{
				insone=strtok(insin," ");
				instwo=strtok(NULL,"\n");
				break;
			}
		}
		if(i==len)
		{
			insone=strtok(insin,"\n");
			instwo=NULL;
		}
		int retins;
		retins=inscomfirm(insone);//指令处理函数,发送到服务器端
	//	printf("%d\n",retins);
		if(retins==-1)
		{
			printf("error instruction.\n");
		}
		if(retins==1)
		{
			break;
		}else
		{
			insmes.ins=retins;
			if(instwo!=NULL)
			{
				insmes.len=strlen(instwo);
				strcpy(insmes.buf,instwo);
				send(sfd,&insmes,insmes.len+8,0);//fa song jie gou ti.
			//	send(sfd,&insmes,sizeof(insmes),0);
			}else{
				insmes.len=0;
				send(sfd,&insmes,8,0);
			//	send(sfd,&insmes,sizeof(insmes),0);
			}
			switch(insmes.ins)
			{
				//printf("%d\n",insmes.ins);
				case 2:
					recv_cdresult(sfd); break;
				case 3:
					recv_lsresult(sfd); break;
				case 4://puts
					//执行puts之前要发送文件类型和md5码
					//在send_file函数里进行这波操作
				//	send_n(sfd,mymd5,sizeof(md5));//我这边发md5码，那边接
					send_file(sfd,insmes.buf); break;
				case 5:
					if(instwo==NULL)
					{
						goto start;
					}
					recv_n(sfd,path,sizeof(path));
					recv_getsresult(sfd,path); break;
				case 6:
					recv_removeresult(sfd); break;
				case 7:
					recv_pwdresult(sfd); break;
				case 8:
					recv_mkdir(sfd);//接收创建文件夹是否成功
					break;
				default:
					break;
			}
		}
	}
	close(sfd);
	return 0;
}




















