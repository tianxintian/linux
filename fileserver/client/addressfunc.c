#include"func.h"
//命令配对函数，返回的数字提供给客户端和用户switch进行选择处理
int inscomfirm(char *insone)
{
	if(!strcmp(insone,"exit")) return 1;
	if(!strcmp(insone,"cd")) return 2;
	if(!strcmp(insone,"ls")) return 3;
	if(!strcmp(insone,"puts")) return 4;
	if(!strcmp(insone,"gets")) return 5;
	if(!strcmp(insone,"remove")) return 6;
	if(!strcmp(insone,"pwd")) return 7;
	if(!strcmp(insone,"mkdir")) return 8;
	else{
		return -1;
	}
}

//小火车发送数据，发送文件的时候调用,可以发送连续空间，比如传输结构体
int send_n(int sfd,char *buf,int len)
{
	int total=0;
	int ret;
	while(total<len)
	{
		ret=send(sfd,buf+total,len-total,0);
		if(ret==-1)
		{
			printf("client close\n");
			break;
			return -1;
		}
		total=total+ret;
	}
	return 0;
}

//小火车接收数据，接受文件的时候调用
int recv_n(int sfd,char* buf,int len)
{
	int total=0;
	int ret;
	while(total<len)
	{
		ret=recv(sfd,buf+total,len-total,0);
		if(ret==0)
		{
			printf("server close\n");
			return -1;
		}
		total=total+ret;
	}
	return 0;
}

//发送文件的函数，传入网络描述符和文件名
int send_file(int new_fd,char *FILENAME)
{
	char path[177]={0};
	char buf1[177]={0};
	recv_n(new_fd,path,sizeof(path));
	recv_n(new_fd,buf1,sizeof(buf1));
	strcat(path,buf1);
	chdir(path);
	printf("zai zhe fa %s\n",path);
	signal(SIGPIPE,SIG_IGN);

	//在这里添加获取文件类型和文件md5码
	int ret1,ret2;
	char mymd5[177]={0};
	char type[7]={0};//保存文件类型
	ret1 = get_md5(FILENAME,mymd5);
	send_n(new_fd,mymd5,sizeof(mymd5));//发送给客户端md5码
	struct stat buffile;//这里获取文件的类型
	stat(FILENAME,&buffile);
	if(S_ISDIR(buffile.st_mode))
	{
		strcpy(type,"d");
	}else
	{
		strcpy(type,"-");
	}
	send_n(new_fd,type,sizeof(type));//发送文件类型
	recv_n(new_fd,(char*)&ret2,sizeof(ret2));
	printf("ret2 is %d\n",ret2);
	if((int)ret2==1)//表示服务器说自己有了这个文件，你不用上传了，我这边帮你
		//搞成硬链接就行了，你直接结束这个程序片段就行了
	{
		printf("wocongzhefasong\n");
		train d;
		strcpy(d.buf,FILENAME);
		d.len=strlen(d.buf);
		send(new_fd,&d,4+d.len,0);
		goto end;
	}else
	{
		printf("kaishifalelelele\n");
		train d;
		strcpy(d.buf,FILENAME);
		d.len=strlen(d.buf);
		send(new_fd,&d,4+d.len,0);//这里是发送文件名长度和文件名
		int fd;
		fd=open(FILENAME,O_RDONLY);
		if(fd==-1)
		{
			perror("open");
		}
		struct stat buf;
		int ret=fstat(fd,&buf);
		if(ret==-1)
		{
			perror("fstat");
		}
		d.len=sizeof(off_t);
		memcpy(d.buf,&buf.st_size,d.len);
		send_n(new_fd,(char*)&d,4+d.len);
		while((d.len=read(fd,d.buf,sizeof(d.buf)))>0)
		{
			ret=send_n(new_fd,(char*)&d,4+d.len);
			if(ret==-1)
			{
			}
		}
		d.len=0;
		send_n(new_fd,(char*)&d,4);
		close(fd);
	}
end:
	return 0;
}

//接受文件函数,传入连接的网络描述符sfd即可获取下载文件
void recv_file(int sfd,char *path)
{
	chdir(path);//定位到这个路径下接收文件
	int len;
	int ret;
	off_t filesize;//已经接收了大小
	char buf[1000]={0};
	ret=recv(sfd,&len,sizeof(len),0);//接收文件名的长度
	if(ret==0)
	{
		printf("server close");
		close(sfd);
	}
	ret=recv(sfd,buf,len,0);//接收文件名
	if(ret==0)
	{
		printf("server close\n");
		close(sfd);
	}
	int fd;
	fd=open(buf,O_RDWR|O_CREAT,0666);
	//断点下载
	if((filesize=is_exist(buf))!=0){
		printf("download from last position\n");
		lseek(fd,ret,SEEK_SET);
	}
	send_n(sfd,(char*)&filesize,sizeof(off_t));//发送已经接收了的文件大小
	off_t f_size;
	ret=recv_n(sfd,(char*)&len,sizeof(len));//接收文件大小的数字长度
	if (ret==-1)
	{
		printf("server close\n");
		close(fd);
	}
	ret=recv_n(sfd,(char*)&f_size,len);//接收该接收的总文件大小放入f_size
	printf("wenjiandaxiaoshi %ld\n",f_size);
	if(ret==-1)
	{
		printf("server close\n");
		close(fd);
	}
	if(f_size<(long)pow(2,7))
	{
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
	}else//当文件大于128M的时候接收
	{
		//接收代码写在这里，ret返回接收的字节数
		//最多设置为接收1000个字节
		printf("kaishi jieshou\n");
		int i;
		int N=(f_size-filesize)/1000;
		for(i=0;i<N;i++)
		{
			ret=recv(sfd,buf,sizeof(buf),0);
			printf("ret is%d\n",ret);
			//while(1);
			write(fd,buf,ret);//写入文件
			bzero(buf,sizeof(buf));
		}
		ret=recv(sfd,buf,sizeof(buf),0);
		write(fd,buf,ret);
		printf("load success\n");
	}
end:
	close(fd);
}

//接收cd后的路径并显示
int recv_cdresult(int sfd)
{
	char path[177];
	bzero(path,sizeof(path));
	recv(sfd,path,sizeof(path),0);
	printf("%s\n",path);
}


//上传文件处理函数puts 
void recv_putsresult(int sfd,char *FILENAME)
{
	send_file(sfd,FILENAME);
}

//gets命令的接收处理函数
void recv_getsresult(int sfd,char* path)
{
	recv_file(sfd,path);
}

//pwd命令查看当前路径
int recv_pwdresult(int sfd)
{
	int len;
	char buf[77];
	recv(sfd,&len,4,0);
	bzero(buf,sizeof(buf));
	recv(sfd,buf,len,0);
	printf("当前路径为:");
	printf("%s\n",buf);
	return 0;
}

struct ls_m{
	char type;
	long size;
	char name[20];
	int flag;//判断结束标志
};
//接收ls命令处理结果
int recv_lsresult(int sfd)
{
	char name[10]="name";
	char type[10]="file-type";
	char size[10]="size";
	printf("%s      %-21s  %s\n",type,name,size);
	while(1)
	{   
		struct ls_m msg;
		recv_n(sfd,(char*)&msg,sizeof(msg));
		//recv(sfd,&msg,sizeof(msg),0);
		if(msg.flag==1)
		{
			break;
		}else
		{
			printf("    %c       %-20s      %ldB\n",msg.type,msg.name,msg.size);
		}	
	}
	return 0;
}

//接收remove函数的结果
int recv_removeresult(int sfd)
{
	int flag;//判断删除成功和输出标志
	recv_n(sfd,(char*)&flag,sizeof(flag));
	if(flag==7)
	{
		printf("remove success\n");
		return 0;
	}
	if(flag==10)
	{
		printf("remove fail\n");
		return 0;
	}
}

//看文件是否存在，是则返回文件大小
off_t is_exist(char *filename)
{
	DIR* dir=opendir(".");
	off_t filesize=0;
	struct stat fstat;
	struct dirent *p;
	while((p=readdir(dir))!=NULL)
	{
		if(!strcmp(p->d_name,filename))
		{
			stat(filename,&fstat);
			filesize=fstat.st_size;
			break;
		}
	}
	return filesize;
}

//计算文件的MD5码,传入文件名和存储md5码的地址
int get_md5(char* FileName,unsigned char* myMD5)  
{  
	MD5_CTX md5;  
	unsigned char outmd[16];  
	char buffer[1024];
	char buf[33]={0};
	char tmp[3]={0};
	int len=0;  
	int i;  
	FILE * fp=NULL;  
	memset(outmd,0,sizeof(outmd));  
	memset(buffer,0,sizeof(buffer));  
	fp=fopen(FileName,"rb");  
	if(fp==NULL)  
	{  
		printf("Can't open file\n");  
		return 0;  
	}  

	MD5_Init(&md5);  
	while((len=fread(buffer,1,1024,fp))>0)  
	{  
		MD5_Update(&md5,buffer,len);  
		memset(buffer,0,sizeof(buffer));  
	}  
	MD5_Final(outmd,&md5);  

	for(i=0;i<16;i<i++)  
	{  
		sprintf(tmp,"%02X",outmd[i]);
		strcat(buf,tmp);
	}
	strcpy(myMD5,buf);
	len=strlen(myMD5);//将获取的md5码农从小写转大写
	for(i=0;i<len;i++)
	{
		if(myMD5[i]>='A'&&myMD5[i]<='Z')
		{
			myMD5[i]=myMD5[i]+32;
		}
	}
	return 0;  
}


void recv_mkdir(int sfd)//用来接收mkdir返回信息
{
	char buf[177]={0};
	recv_n(sfd,buf,sizeof(buf));
	printf("%s\n",buf);
}
















