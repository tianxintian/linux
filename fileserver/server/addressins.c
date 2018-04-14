#include"factory.h"
//发送给sfd缓冲buf中len个字节，发送文件调用

int send_n(int sfd,char* buf,int len)
{
	int total=0;
	int ret;
	while(total<len)
	{
		ret=send(sfd,buf+total,len-total,0);
		if(ret==-1)
		{
			printf("client close\n");
			return -1;
		}
		total=total+ret;
	}
	return 0;
}

//接受sfd发来的len个字节放入缓冲区buf里，接收文件调用
int recv_n(int sfd,char* buf,int len)
{
	int total=0;
	int ret;
	while(total<len)
	{
		ret=recv(sfd,buf+total,len-total,0);
		total=total+ret;
	}
	return 0;
}

//发送文件的函数，传入网络描述符和文件名
//服务器端改造成mmap，sendfile包含mmap
//当然客户端接收文件的函数肯定要相应的修改啊
//大于100M的时候用sendfile，然后发送
//小于100M用小火车发送

/*
   ssize_t sendfile(int out_fd, int in_fd, off_t *offset, size_t count);

   sendfile()是作用于数据拷贝在两个文件描述符之间的操作函数.
   这个拷贝操作是内核中操作的,所以称为"零拷贝".
   sendfile函数比起read和write函数高效得多,使用了mmap.
   因为read和write是要把数据拷贝到用户应用层操作.

   参数说明:
   out_fd 是已经打开了,用于写操作(write)的文件描述符;
   in_fd 是已经打开了,用于读操作(read)的文件描述符;
   offset 偏移量;表示sendfile函数从in_fd中的哪一偏移量开始读取数据.
   如果是零表示从文件的开始读,否则从相应的便宜量读取.如果是循环读取的时候,
   下一次offset值应为sendfile函数返回值加上本次的offset的值.
   count是在两个描述符之间拷贝的字节数(bytes)

   返回值:
   如果成功的拷贝,返回写操作到out_fd的字节数,错误返回-1,并相应的设置error信息.
   */
/*int sendfile_n(int sfd,int fd,size_t size)
  {
  size_t total=0;//总的发送了多少B
  size_t num;//每次发送了多少B
  while(total<size)
  {
  num=sendfile(sfd,fd,NULL,size-total);//NULL可以使fd自动偏移
  if(num==0)                          //其他的不可以
  {
  return total;//发送成功，但是没有1000B可以发了
  }//返回发送的总量，客户端接收到就完了
  if(num==-1)//发送失败返回-1
  {
  return -1;
  }
  total+=num;
  }
  return 1000;
  }
  */
void send_file(int new_fd,char *FILENAME)
{
	off_t filesize;//发送了的大小
	signal(SIGPIPE,SIG_IGN);
	train d;
	strcpy(d.buf,FILENAME);
	d.len=strlen(d.buf);
	send(new_fd,&d,4+d.len,0);
	int fd;
	fd=open(FILENAME,O_RDONLY);
	if(fd==-1)
	{
		perror("open");
	}
	recv_n(new_fd,(char*)&filesize,sizeof(off_t));//发送了的大小
	printf("yi fasong %ld\n",filesize);
	lseek(fd,filesize,SEEK_SET);//接着上次发过后
	//的地方继续发，也即断点续传
	struct stat buf;
	int ret;
	ret=fstat(fd,&buf);
	if(ret==-1)
	{
		perror("fstat");
	}
	d.len=sizeof(off_t);
	memcpy(d.buf,&buf.st_size,d.len);//buf.size表示文件的大小
	//与已经发送的文件的大小filesize不一样
	send_n(new_fd,(char*)&d,4+d.len);
	if(buf.st_size<(long)pow(2,27))
	{//当文件的大小小于128M的时候用小火车发送
		while((d.len=read(fd,d.buf,sizeof(d.buf)))>0)
		{
			ret=send_n(new_fd,(char*)&d,4+d.len);
			if(ret==-1)
			{
				break;
			}
		}
		d.len=0;
		send_n(new_fd,(char*)&d,4);
	}else
	{//当文件大小大于128M的时候用带有mmap功能的sendfile函数发送
		printf("sendfile fasong\n");
		while(1)
		{
			ret=sendfile(new_fd,fd,NULL,1000);
			printf("ret is %d\n",ret);
			if(ret<=0)
			{
				printf("chuanshu jieshu.\n");
				goto end;
				break;
			}
		}
		/*printf("ret is%d\n",ret);
		  if(ret==-1||ret==0)
		  {
		  goto end;
		  }*/
	}
end:
	close(fd);
}

//接受文件函数,传入连接的网络描述符sfd即可获取下载文件
void recv_file(int sfd,char *filename,char*pathser)
{
	chdir(pathser);
	int len;
	int ret;
	char buf[1000]={0};
	/*ret=recv(sfd,&len,sizeof(len),1);
	  if(ret==0)
	  {
	  printf("server close1");
	  }
	  ret=recv(sfd,buf,len,1);
	  if(ret==0)
	  {
	  printf("server close2\n");
	  }*/
    //这块也可以放在case那主线程那里进行操作
	//在这里要添加数据库操作了，检查当前路径，查询数据库，插入数据库（即虚拟路径），或者建立
	//硬链接再插入数据库，这就是所谓的不上传，然后直接跳过下载即可，因为服务器上已经存在了
	//在数据库中查询文件是否存在

	int fd;
	fd=open(filename,O_RDWR|O_CREAT,0666);
	off_t f_size;
	ret=recv_n(sfd,(char*)&len,sizeof(len));
	if (ret==-1)
	{
		printf("server close\n");
		close(fd);
	}
	ret=recv_n(sfd,(char*)&f_size,len);
	if(ret==-1)
	{
		printf("server close\n");
		close(fd);
	}
	float f=0;
	int j=1;
	time_t now,last;
	now=time(NULL);
	last=now;
	printf("loaded:\n");
	while(1)
	{
		ret=recv_n(sfd,(char*)&len,sizeof(len));
		if(ret==-1)
		{
			printf("%5.2f%s\n",f/f_size*100,"%");
			break;
		}
		else if(len>0)
		{
			f=f+len;
			time(&now);
			if(now-last>=1)
			{
				printf("%5.2f%s\r",f/f_size*100,"%");
				fflush(stdout);
				last=now;
			}
			ret=recv_n(sfd,buf,len);
			write(fd,buf,len);
			if(ret==-1)
			{
				printf("%5.2f%s\n",f/f_size*100,"%");
				break;
			}
		}else
		{
			printf("       \r");
			printf("%d%s\n",100,"%");
			break;
		}
	}
	close(fd);
}

int send_pwd(int sfd,char *path,char *buf)
{
	char root[1024];
	getcwd(root,sizeof(root));
	//strcat(path,buf);
	//chdir(path);
	train t;
	bzero(t.buf,sizeof(t.buf));
	strcpy(t.buf,buf);
	t.len=strlen(t.buf);
	send(sfd,&t,4+t.len,0);
	chdir(root);
	return 0;
}

int cd_address(int sfd,char* name,char* buf)
{
	//	char buf[177];
	//	bzero(buf,sizeof(buf));
	//	strcmp(buf,path);
	int len=strlen(buf);
	int i,count=0;//count为/的数目
	for(i=0;i<len;i++)
	{
		if(buf[i]=='/')
		{
			count++;
		}
	}
	if(!strcmp(name,"/"))
	{
		buf[1]='\0';
		send(sfd,buf,sizeof(buf),0);
		goto end;
	} 
	if(!strcmp(name,".."))//后退
	{
		int count1=0;
		if((count1+1)==count)//只有一个/的时候,也就是根目录下，无法后退
		{
			buf[1]='\0';
			send(sfd,buf,1,0);
		}else{//不止一个/的时候
			for(i=0;i<len;i++)
			{
				if(buf[i]=='/')
				{
					count1++;
					if(count1==count)
					{
						buf[i]='\0';
						send(sfd,buf,i,0);
						break;
					}
				}
			}
		}
	}
	if((strcmp(name,".")!=0)&&(strcmp(name,"..")!=0))
	{
		if(len>1)//也就是说/user这样类型才拼接，只有/不拼
		{
			strcat(buf,"/");
		}
		strcat(buf,name);
		int len1=strlen(buf);
		send(sfd,buf,len1,0);
	}
end:
	return 0;
}


int ls_address(int sfd,char* path){//ls处理函数
	char root[1024];
	getcwd(root,sizeof(root));
	chdir(path);
	DIR* dir=opendir(path);
	struct stat buf;
	if(dir==NULL)
	{
		perror("opendir");
		return -1;
	}
	struct dirent* p;
	struct ls_m msg;
	while((p=readdir(dir))!=NULL)//统计文件个数
	{
		if(!strcmp(p->d_name,".")||!strcmp(p->d_name,".."))
		{	
		}else
		{
			bzero(&msg,sizeof(msg));
			struct stat buf;
			stat(p->d_name,&buf);
			if(S_ISDIR(buf.st_mode))
			{
				msg.type='d';
			}else{              
				msg.type='-';
			}
			strcpy(msg.name,p->d_name);
			int len=strlen(msg.name);
			msg.size=buf.st_size;
			msg.flag=7;//随意设置的标志，只要传过去不为1即可
			send_n(sfd,(char*)&msg,sizeof(msg));
		}
	}
	chdir(root);
	msg.flag=1;//控制接收端，接收端接到0就退出 或者加个flag，为1就断
	send_n(sfd,(char*)&msg,sizeof(msg));
	closedir(dir);
	return 0;
}


int send_remove(int sfd,char *name,char*path)
{
	int flag=0;//成功或者失败标志
	char root[1024];
	getcwd(root,sizeof(root));
	chdir(path);
	if(unlink(name)!=0)
	{
		flag=10;//flag等于10为成功
		send_n(sfd,(char*)&flag,sizeof(flag));
		chdir(root);
		return 0;
	}else
	{
		flag=7;//flag等于7表示成功
		send_n(sfd,(char*)&flag,sizeof(flag));
		chdir(root);
		return 0;
	}
}

void mkdir_address(int sfd,char *path,char*instwo)
{
	char buf[177]={0};//发送给客户端文件夹创建信息
	char bufpath[177]={0};
	sprintf(bufpath,"%s/%s",path,instwo);
	strcpy(buf,"mkdir success.");
	DIR *dir=opendir(bufpath);
	if(dir!=NULL)//文件夹已经存在
	{
		bzero(buf,sizeof(buf));
		strcpy(buf,"mkdir fail.");
		send_n(sfd,buf,sizeof(buf));
	}
	int ret=mkdir(bufpath,0777);
	if(ret==-1)
	{
		perror("mkdir");
	}
	send_n(sfd,buf,sizeof(buf));
}

//在获取precode值的时候要先判断文件存在与否，存在就创建硬链接，
//保存abspath
int ret_precode(char *path,char *buf,char *username)//path为当前路径，buf为保存文件名
{
//	char bufpath[177]={0};
//	strcpy(bufpath,path);
//	
//	char buf1[77]={0};
//	printf("ret_precode path is %s\n",path);
	if(!strcmp(path,"/"))
	{
		return 0;
	}else
	{
		//添加到数据库中的查询操作，并添加字符串分割操作
		//获取最后一个/后的字符串，调用查询函数到数据库中查询
		//要写一个数据库查询函数，从数据库中获取code值作为precode值
		//这里操作字符串分割和查询可以写成两个函数
		//不急，每天有时间就写一点。
		int i;
		int ret;
		int count1=0;
		int count2=0;
		int len = strlen(path);
		for(i=0;i<len;i++)
		{
			if(path[i]=='/')
			{
				count1++;
			}
		}
		for(i=0;i<len;i++)
		{
			printf("buxiangcuoleo \n");
			if(path[i]=='/')
			{
				count2++;
				if(count1==count2)
				{
					strcpy(buf,path+i+1);
			//		printf("is %s\n",path+i+1);
					//这里写一个函数，到数据库中去查，返回code值
					printf("buf is %s\n",buf);
					ret=findcode(buf,username);
					return ret;
				}
			}
		}
	}
}



int findcode(char *name,char *username)
{
	int ret;
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;//这里就不定义登录注册的那个全局变量来
	//因为这里就一个但查询，要修改的不多
	char* server="localhost";
	char* user="root";
	char* password="123";
	char* database="fileserver";
	char query[300]={0};
	sprintf(query,"select * from file where name='%s' and user='%s'",name,username);
//	strcpy(query,"select * from file where name='");
//	strcat(query,name);
//	strcat(query,"'");
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
	t=mysql_query(conn,query);//执行查询操作，数据库肯定有的
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(conn));
	}else
	{
		res=mysql_use_result(conn);
		if(res)//实际不用加判断，肯定为真
		{
			if((row=mysql_fetch_row(res))!=NULL)
			{
				sscanf(row[0],"%d",&ret);
				printf("ret is %d\n",ret);
				return ret;//row[1]即为code值
			}
		}
	}
}//但是这个函数没有写关闭和free函数释放res和conn，暂时未解决。



int file_exist(char *md5)//在数据库中查询文件是否存在
{
	int ret;//如果数据库中存在这个文件了，就返回1，否则返回0
	//如果文件存在，建立硬链接，保存路径即可，这样就不用大改代码
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char* server="localhost";
	char* user="root";
	char* password="123";
	char* database="fileserver";
	char query[300]={0};
	strcpy(query,"select * from file where md5='");
	strcat(query,md5);
	strcat(query,"'");
	puts(query);
	int t;
	conn=mysql_init(NULL);
	if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
	{
		printf("Error connecting to database:%s\n",mysql_error(conn));
	}else
	{
		printf("Connected...\n");
	}
	t=mysql_query(conn,query);//执行查询语句
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(conn));
	}else
	{
		res=mysql_use_result(conn);
		if(res)
		{
			if((row=mysql_fetch_row(res))!=NULL)
			{
				return 1;
			}else
			{
				return 0;
			}
		}
	}
}

void file_insert(int precode,char *fname,char *type,char *md5,char *fuser,char *abspath)
{
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char* server="localhost";
	char* user="root";
	char* password="123";
	char* database="fileserver";
	char query[300]={0};
	printf("filename is %s\n",fname);
	sprintf(query,"insert into file(precode,name,type,md5,user,abspath) values('%d','%s','%s','%s','%s','%s')",precode,fname,type,md5,fuser,abspath);
	int t;
	conn=mysql_init(NULL);
	if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
	{
		printf("Error connecting to database:%s\n",mysql_error(conn));
	}else
	{
		printf("Connected...\n");
	}
	t=mysql_query(conn,query);//执行插入操作
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(conn));
	}else
	{
		//什么都不做，已经能够插入完成了。
	}
}

void find_abspath(char *md5,char *abspath)
{
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char* server="localhost";
	char* user="root";
	char* password="123";
	char* database="fileserver";
	char query[300]={0};
	sprintf(query,"select * from file where md5='%s'",md5);
	int t;
	conn = mysql_init(NULL);
	if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
	{
		printf("Error connecting to database:%s\n",mysql_error(conn));
	}else
	{
		printf("Connected...\n");
	}
	t=mysql_query(conn,query);
	printf("kale1\n");
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(conn));
	}else
	{
		printf("kale2\n");
		res=mysql_use_result(conn);
		if(res)
		{
			printf("kale3\n");
			if((row=mysql_fetch_row(res))!=NULL)
			{
				printf("abspath is %s\n",row[6]);
				strcpy(abspath,row[6]);
			}
		}
	}
	mysql_free_result(res);
	mysql_close(conn);
}
