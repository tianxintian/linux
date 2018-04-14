#include"factory.h"
void *thread_handle(void *p)
{//子线程和客户端对话函???
//	char path[177];//保存每个线程进来的路???
//	char bufip[77]={0};//保存客户的ip地址
//	bzero(path,sizeof(path));
	pfac pf=(pfac)p;
//	strcpy(bufip,pf->bufip);//把客户的ip地址存储在bufip里
	pque_t pq=&pf->que;
	pnode_t pcur;
	//strcpy(path,pf->buf);//把主线程的路径传进来，给子线程修???
//	getcwd(path,sizeof(path));
//	strcat(path,"/user");//直接定位到user目录，服务器上自己要知道有哪些文件
	//int sfd=pcur->new_fd;
	while(1)
	{
		//int len=0;
	//	char path[100];
		//bzero();
		pthread_mutex_lock(&pq->que_mutex);
		if(!pq->que_size)
		{
			pthread_cond_wait(&pf->cond,&pq->que_mutex);
		}
		que_get(pq,&pcur);
		pthread_mutex_unlock(&pq->que_mutex);
	//	getcwd(path,sizeof(path));
		//在这里添加处理代码
		int sfd=pcur->new_fd;
		//注册登录
		int xuanze;
		int ret;
		char username[17];
flag:
		recv_n(sfd,(char*)&xuanze,sizeof(xuanze));
		switch(xuanze)
		{
			case 1://登录
delu:				
			bzero(username,sizeof(username));
			ret=login(sfd,username);//在函数里加个功能，这个功能就是判断
			//是否存在这个用户文件夹，如果没有的话，就创建文件夹。
			//就是以后就在这个路径底下进行操作，特别是puts的时候那个虚拟目录
			if(ret==33)//表示用户名正确，密码错了三次，重新进行选择1或者2
			{//进行操作
				goto flag;
			}
			//测试能不能获得用户名
			printf("username is %s\n",username);
				break;
			case 2://注册
				printf("zhuce laile.\n");
				ret=regis(sfd);
				goto delu;
				break;
		}
		printf("已经准备好接收指令\n");
		char buf[177]={0};
		strcpy(buf,"/");//保存根路径
		while(1){
			char path[177]={0};
			char pathser[177]={0};
			strcpy(pathser,"/home/luke/fileserver2/server");
			strcpy(path,"/home/luke/fileserver2/server/user/");
			strcat(path,username);
			int ins;//指令类型
			int len;//指令长度
			char instwo[77]={0};//指令第二部分
			recv(sfd,&ins,4,0);
		//	printf("%d\n",ins);
			recv(sfd,&len,4,0);
		//	printf("%d\n",len);
			if(len!=0)
			{
				recv_n(sfd,instwo,len);
			}
			int len1;//接受文件的时候文件名长度
			int ret1;//接收文件的返回
			int precode;//接收文件的precode
			char type[7]={0};//接收客户端发来的文件类型，puts时调用的
			char md5[177]={0};//接收客户端发来的文件的md5码，puts时调用
			char buf1[1000]={0};//接收客户端发来的文件名用的
			char bufbk[1000]={0};//备份文件名
			char bufbaocun[77]={0};//存最后一个文件名
			char bufpret[77]={0};//保存字符分隔的字符串
			char abspath[177]={0};//保存从数据库中查询得到的路径，已经存在文件的路径
			switch(ins)
			{
		//		printf("%d\n",ins);
				case 1://exit 
					syslog(LOG_INFO,"client excute ins exit.");
					printf("exit\n");
					while(1);
					break;
				case 2://cd
					 syslog(LOG_INFO,"client excute ins cd.");
					cd_address(sfd,instwo,buf);
					break;
				case 3://ls
					syslog(LOG_INFO,"client excute ins ls.");
					strcat(path,buf);
					printf("lujing is %s\n",path);
					ls_address(sfd,path);
					break;
				case 4://puts 
					syslog(LOG_INFO,"client excute ins puts.");
					send_n(sfd,path,sizeof(path));//让客户端写死的路径拼接
					send_n(sfd,buf,sizeof(buf));//当前路径，切换到这个再发
				//	ret1=recv(sfd,&len1,sizeof(len),0);//接收文件名长度
				//	ret1=recv(sfd,buf1,len1,0);//接收文件名到buf1
					//这里和客户端交互，接收文件类型和文件md5码
					recv_n(sfd,md5,sizeof(md5));
					recv_n(sfd,type,sizeof(type));
			        ret1=file_exist(md5);//表示文件是否存在
					//把ret1发过去，让客户端根据ret1的值选择执行不同的操作
					send_n(sfd,(char*)&ret1,sizeof(ret1));
					printf("ret1 is %d\n",ret1);

					if((int)ret1==1)//表示文件存在数据库里
					{
						ret1=recv(sfd,&len1,sizeof(len),0);
						ret1=recv(sfd,buf1,len1,0);
						printf("wenjian ming is %s\n",buf1);
						strcpy(bufbk,buf1);
						char newpath[177]={0};
						sprintf(newpath,"%s%s",path,buf);
						sleep(2);
						find_abspath(md5,abspath);//返回绝对路径到abspath
						strcpy(bufpret,buf);
						precode=ret_precode(bufpret,bufbaocun,username);
						link(abspath,newpath);
						file_insert(precode,bufbk,type,md5,username,newpath);
						printf("charuwanchenglelellelelel\n");
						//再定义一个函数，从数据库中查询返回abspath
						//这里执行硬链接，要从数据库查询路径，并插入
						//当然之前要获取precode的值
						//并goto到程序结尾，因为不需要接收文件了
						//同时客户端也要结束执行，以免卡住程序，执行不了下一步
					}else//表示文件不存在数据库里
					{
						//这里执行插入操作，获取precode的值
					//	precode=ret_precode();
						ret1=recv(sfd,&len1,sizeof(len),0);//接收文件名长度
						ret1=recv(sfd,buf1,len1,0);//接收文件名到buf1
						strcpy(bufbk,buf1);//对文件名进行备份，防止被修改
						strcpy(bufpret,buf);//把当前路径保存的给bufpret
						precode=ret_precode(bufpret,bufbaocun,username);//根据当前路径和文件名获取precode值
						//while(1);
						strcat(path,buf);//拼接成绝对路径
						file_insert(precode,bufbk,type,md5,username,path);
						recv_file(sfd,bufbk,pathser);//接收文件
					}
					break;
				case 5://gets
					syslog(LOG_INFO,"client excute ins gets.");
				   if(len==0)
				   {
					   break;
				   }
					strcat(path,buf);
			 		send_n(sfd,path,sizeof(buf));//把路径发过去
					//让客户端自己想在对应的路径下下载文件		
					send_file(sfd,instwo);
					printf("chulaile\n");
					break;
				case 6://remove
					syslog(LOG_INFO,"client excute ins remove.");
					strcat(path,buf);
					send_remove(sfd,instwo,path);				
					break;
				case 7://pwd
					 syslog(LOG_INFO,"client excute ins pwd.");
					send_pwd(sfd,path,buf);
					break;
				case 8:
					syslog(LOG_INFO,"client excute ins mkdir.");
					strcat(path,buf);
					mkdir_address(sfd,path,instwo);
				default:
					if(ins==0)
					{
						goto end;
					}
					break;
			}
		}
	}
end:
	close(pcur->new_fd);//退出的时候关
	free(pcur);//释放取节点的空间
}
int main(int argc,char *argv[])
{
	printf("started\n");
	if(argc!=5)
	{
		printf("./server ip port thread_num capacity\n");
		return -1;
	}
//	char buf[177]={0};
//	getcwd(buf,sizeof(buf));
	fac f;//工厂数据结构???
	bzero(&f,sizeof(f));
	f.pthread_num=atoi(argv[3]);
	int capacity=atoi(argv[4]);
//	strcpy(f.buf,buf);//把主线程路径传给每个子进程保存好
	factory_init(&f,thread_handle,capacity);
	int sfd=socket(AF_INET,SOCK_STREAM,0);
	if(sfd==-1)
	{
		perror("socket");
		return -1;
	}
	int reuse=1;
	int ret;
	ret=setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(int));
	if(ret==-1)
	{
		perror("setsockopt");
		return -1;
	}
	struct sockaddr_in seraddr,client;//client用来接收客户端ip和端口号
	bzero(&seraddr,sizeof(seraddr));
	seraddr.sin_family=AF_INET;
	seraddr.sin_port=htons(atoi(argv[2]));
	seraddr.sin_addr.s_addr=inet_addr(argv[1]);
	ret=bind(sfd,(struct sockaddr*)&seraddr,sizeof(seraddr));
	if(ret==-1)
	{
		perror("bind");
		return -1;
	}
	factory_start(&f);//创建线程
	listen(sfd,100);
	int new_fd;
	pnode_t pnew;
	pque_t pq=&f.que;
	while(1)
	{
		int size=sizeof(struct sockaddr);
		bzero(&client,sizeof(struct sockaddr_in));
		new_fd=accept(sfd,(struct sockaddr*)&client,&size);
//		strcpy(f.bufip,inet_ntoa(client.sin_addr));
		//记录日志信息
		syslog(LOG_INFO,"client'ip is %s connect.",inet_ntoa(client.sin_addr));
		pnew=(pnode_t)calloc(1,sizeof(node_t));
		pnew->new_fd=new_fd;
		pthread_mutex_lock(&pq->que_mutex);
		que_set(pq,pnew);
		pthread_mutex_unlock(&pq->que_mutex);
		pthread_cond_signal(&f.cond);
	}
}
























