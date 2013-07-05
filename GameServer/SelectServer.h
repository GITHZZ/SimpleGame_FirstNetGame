/*
 *  SelectServer.h
 *  GameServer
 *
 *  Created by studentg07 on 12-4-7.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

/*
 *  SelectServer.h
 *  GameServer
 *
 *  Created by studentg07 on 12-4-7.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */
#include <stdio.h> /* These are the usual header files */
#include <strings.h> /* for bzero() */
#include <unistd.h> /* for close() */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <stdlib.h>/*for malloc() free()等*/
#include "MsgDefine.h"
#include <iostream>
using namespace std;
#include <vector>
#define PORT 8000 /* 定义端口号 */
#define BACKLOG 10 /* 最大同时允许连接的数量 */
#define MAXDATASIZE 1024

typedef struct CLIENT{
	int fd;//连接套接字
	char* name;//客户名
	struct sockaddr_in addr; /* 客户地址 */
	char* data;//客户数据
};


class SelectServer
{
private:
	int maxi;//maxi为最后一个描述符编号,这里作全局变量目的是供其它函数使用
	fd_set rset, allset;//rset为可读描述符集合，allset为所有描述符集合
	int gameState;
		//声明savedata()函数，用于实现存储客户数据。
	void savedata(char* recvbuf, int len, char* data);
public:
	/* 客户端信息数组, 作全局变量目的是供其它函数使用*/
	struct CLIENT client[FD_SETSIZE];//FD_SETSIZE为系统内部常量，为1024
	
	//发送消息给某个套接字
	void sendMsg(int fd,char* buf,int len);
	//发送消息给所有与客户端连接的套接字
	void sendMsg(char*buf,int len);
	//实现轮流控制
	void mySendMsg(char*buf,int len);
	//开始服务器
	void startSelectServer();
	void deleteClient(struct CLIENT *client);//断开连接并删除客户端
protected:	
	//定义虚函数onRead
	virtual void onRead(struct CLIENT *client,char *recvbuf,int nLen){}	
	//定义虚函数onClose
	virtual void onClose(struct CLIENT *client){}
};