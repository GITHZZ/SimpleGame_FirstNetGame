/*
 *  SelectServer.cpp
 *  GameServer
 *
 *  Created by studentg07 on 12-4-7.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "SelectServer.h"

void SelectServer::startSelectServer()
{
	
    // insert code here...
   	
	int i,  maxfd,sockfd;//，maxfd为文件描述符中最大的数字
	int nready;//nready 存放select()函数返回的可读描述符的数量
	ssize_t n;
		int listenfd, connectfd; /* 监听套接字描述符，接收到的套接字描述符 */
	struct sockaddr_in server; /* 服务器地zhi */
	
	char recvbuf[MAXDATASIZE];
	
	gameState=1;
	int sin_size;
	/* 产生TCP 套接字。 */
	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		/* handle exception */
		perror("Creating socket failed.");
		exit(1);
		
	}
	//设置套接字选项为SO_REUSEADDR
	int opt = SO_REUSEADDR;
	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	bzero(&server,sizeof(server));
	server.sin_family=AF_INET;
	server.sin_port=htons(PORT);
	server.sin_addr.s_addr = htonl (INADDR_ANY);
	//绑定套接字到相应地址。本例IP 地址设为INADDR_ANY，可接收来自本机任何IP 地址的客户连接
	if (bind(listenfd, (struct sockaddr *)&server,
			 sizeof(struct sockaddr)) == -1) {
		/* handle exception */
		perror("Bind error.");
		exit(1);
	}
	//监听网络连接
	if(listen(listenfd,BACKLOG) == -1){ /* calls listen() */
		perror("listen() error\n");
		exit(1);
	}
	sin_size=sizeof(struct sockaddr_in);
	/*将所监视的描述符上限（maxfd）设置为监听套接字（listenfd），因为当前listenfd
	 是惟一被监视的描述符。 */
	maxfd = listenfd;
	//将最大连接的客户数（maxi）设置为–1，表示没有连接的客户。
	maxi = -1;
	//将所有客户的连接套接字（fd）设置为–1，表示这些客户还未连接。
	for (i = 0; i < FD_SETSIZE; i++) {
		client[i].fd = -1;
	}
	//清空描述符集（allset）。allset 用于存放所有被监视的描述符。
	FD_ZERO(&allset);
	//将listenfd 放入allset。
	FD_SET(listenfd, &allset);
	while(1)
	{
		struct sockaddr_in addr;
		/*将allset 的数据复制给rset。rset 是可读描述符集合。由于调用select()函数时，系
		 统内核会根据I/O 状态修改rset 的内容，所以必须用allset 来保存所有被监视的描述符，并
		 且在每次调用select()函数前，赋值给rset。*/
		rset = allset;
		/*调用select()函数。由于只监视可读的描述符，可写描述符集合和异常描述符集合
		 被置为NULL。在本例中没有超时限制，将时间参数也置为NULL，nready 存放select()函
		 数返回的可读描述符的数量。*/
		nready = select(maxfd+1, &rset, NULL, NULL, NULL);
		//判断监听套接字是否可读，即是否有新的客户连接。
		if (FD_ISSET(listenfd, &rset)) { 
			/* 接受客户连接。 */
			if ((connectfd = accept(listenfd, (struct sockaddr *)&addr,(socklen_t*)&sin_size))==-1) {
				perror("accept() error\n");
				continue;
			}
			/* 在数组client 寻找一空项，然后初始化新客户信息。 */
			for (i = 0; i < FD_SETSIZE; i++)
				if (client[i].fd < 0) {
					client[i].fd = connectfd; /* save descriptor */
					client[i].name =  (char *)malloc(MAXDATASIZE);  
					client[i].addr = addr;
					client[i].data =  (char *)malloc(MAXDATASIZE);  
					client[i].name[0] = '\0';
					client[i].data[0] = '\0';
					printf("You got a connection from %s.\n ",
						   inet_ntoa(client[i].addr.sin_addr) );
					fflush(stdout);
					
					break;
				}
			//如果客户数达到最大限制，显示错误信息
			if (i == FD_SETSIZE) printf("too many clients\n");
			FD_SET(connectfd, &allset); /* 将新产生的连接套接字放入allset*/
			//如果新产生的连接套接字(connectfd)大于maxfd，则将maxfd 置为connectfd。
			if (connectfd > maxfd) maxfd = connectfd;
			if (i > maxi) maxi = i;
			if (--nready <= 0) continue; /* 如果没有其他可读的描述符（连接套接字），则重新调用select()函数，等待新的
										  可读描述符 */
		}
		for (i = 0; i <= maxi; i++) { /* 检查所有已连接的客户。 */
			if ( (sockfd = client[i].fd) < 0) continue;
			if (FD_ISSET(sockfd, &rset)) {//判断相应客户的连接套接字是否可读。				
				n = recv(sockfd, recvbuf, MAXDATASIZE,0);
				//读客户发来的数据。如果字节数为0，表示客户关闭了连接。
				if ( n == 0) {
					/*处理关闭了连接的客户。关闭套接字；显示客户发来的所有数据；将数据描
					 述符从allset 撤销并将相应客户信息从client 中撤销 */
					onClose(&client[i]);
					close(sockfd);
					printf("Client( %s ) closed connection. \n",
						   client[i].name);
					FD_CLR(sockfd, &allset);
					client[i].fd = -1;
					free(client[i].name);
					free(client[i].data);
				} 
				else
				{
					recvbuf[n] = '\0';
					onRead(&client[i],recvbuf,n);
				}
					//process_cli(&client[i], recvbuf, n);//调用process_cli()函数处理客户请求。
				/* no more readable descriptors */
				if (--nready <= 0) break;
			}
		}
	}
	close(listenfd); /* close listenfd */
}

void SelectServer::deleteClient(struct CLIENT *client){
	
	close(client->fd);
	FD_CLR(client->fd, &allset);
	client->fd = -1;
	free(client->name);
	free(client->data);
}

//发送消息给某个套接字
void SelectServer::sendMsg(int fd,char* buf,int len){
	send(fd,buf,len,0);
	
}

//发送消息给所有与客户端连接的套接字
void SelectServer::sendMsg(char*buf,int len)
{
	for (int i = 0; i <=maxi; i++) 
	{ /* 检查所有已连接的客户。 */
		if ( client[i].fd< 0) 
			continue;
		else 
		{
			send(client[i].fd,buf,len,0);
		}
		
	}
	
}

//实现轮流控制
void SelectServer::mySendMsg(char*buf,int len)
{
	for (int i = 0; i <=maxi; i++) 
	{ /* 检查所有已连接的客户。 */
		if ( client[i].fd< 0) 
			continue;
		else 
		{
			send(client[i].fd,buf,len,0);
		}
		
	}
	
}

//将收到的数据存放在客户信息中
void SelectServer::savedata(char* recvbuf, int len, char* data)
{
	int start = strlen(data);
	for (int i = 0; i < len; i++) {
		data[start + i] = recvbuf[i];
	}
}
