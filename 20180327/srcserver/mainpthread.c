#include"factory.h"
void *thread_handle(void *p)
{//从队列拿一个描述符给子进程去发文件给客户端。
	pfac pf=(pfac)p;
	pque_t pq=&pf->que;
	pnode_t pcur;
	while(1)
	{
		pthread_mutex_lock(&pq->que_mutex);
		if(!pq->que_size)
		{
			pthread_cond_wait(&pf->cond,&pq->que_mutex);
		}
		que_get(pq,&pcur);
		pthread_mutex_unlock(&pq->que_mutex);
		sendfile(pcur->new_fd);
		free(pcur);
	}
}
int main(int argc,char *argv[])
{
	printf("Download service has been opened.\n");
	printf("welcome to download,waiting for you.\n");
	if(argc!=5)
	{
		printf("./server ip port thread_num capacity\n");
		return -1;
	}
	fac f;//工厂数据结构体
	bzero(&f,sizeof(f));
	f.pthread_num=atoi(argv[3]);
	int capacity=atoi(argv[4]);
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
	struct sockaddr_in seraddr;
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
		new_fd=accept(sfd,NULL,NULL);
		pnew=(pnode_t)calloc(1,sizeof(node_t));
		pnew->new_fd=new_fd;
		pthread_mutex_lock(&pq->que_mutex);
		que_set(pq,pnew);
		pthread_mutex_unlock(&pq->que_mutex);
		pthread_cond_signal(&f.cond);
	}
}
























