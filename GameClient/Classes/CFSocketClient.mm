//
//  CFSocketClient.m
//  Lab6CFNetWorkClient
//
//  Created by studentg07 on 12-3-25.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import "CFSocketClient.h"

static void cfCallBack(CFSocketRef cf, CFSocketCallBackType type, 
					   CFDataRef addr, const void *data, void *info) {
	CFSocketClient *client = (CFSocketClient *)info;//获取当前类的对象
	switch (type) {
		case kCFSocketConnectCallBack://连接成功或失败消息
			[client.delegate onConnect:data];
			break;
		case kCFSocketReadCallBack://收到信息消息
		{
			char buf[1024];
			bzero(buf,1024);
			int n=recv(CFSocketGetNative(cf),buf,1024,0);
			buf[n]='\0';
			[client.delegate onRead:buf andLen:n];
		}
			break;
		case kCFSocketWriteCallBack://发送信息消息
			[client.delegate onWrite:data];
			break;
	}
}

@implementation CFSocketClient
@synthesize delegate;
@synthesize bConnected;
- (id)connectServer {
	int bsdSocket=socket(AF_INET,SOCK_STREAM,0);
	// create CFSocket
	CFSocketContext sockContext = { 0, self, NULL, NULL, NULL };
	if (!(cfSocket = CFSocketCreateWithNative(NULL, bsdSocket, 
					kCFSocketReadCallBack | kCFSocketWriteCallBack |
					kCFSocketConnectCallBack, cfCallBack, &sockContext))) {
		return nil;
	}
	
	////////////////////////////设置地址///////////////////	
	NSString *serverAddr = @"127.0.0.1";
	struct   sockaddr_in  addr;
	memset(&addr , 0,sizeof(addr));
	addr.sin_len = sizeof(addr);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8000);
	addr.sin_addr.s_addr = inet_addr([serverAddr  UTF8String]);
	
    CFDataRef address = CFDataCreate(kCFAllocatorDefault,(UInt8*)&addr,sizeof(addr));
	
	/////////////////////////////执行连接/////////////////////
    CFSocketConnectToAddress(cfSocket,address,-1);
	// add to run loop
	CFRunLoopSourceRef source;
	if (!(source = CFSocketCreateRunLoopSource(NULL, cfSocket, 0))) {
		return nil;
	}
	CFRunLoopAddSource(CFRunLoopGetCurrent(), source, kCFRunLoopDefaultMode);
	CFRelease(source);
	
	return self;
}

- (id)connectServer:(NSString *)ip andPort:(int)port {
	int bsdSocket=socket(AF_INET,SOCK_STREAM,0);
	// create CFSocket
	CFSocketContext sockContext = { 0, self, NULL, NULL, NULL };
	if (!(cfSocket = CFSocketCreateWithNative(NULL, bsdSocket, 
											  kCFSocketReadCallBack | kCFSocketWriteCallBack |
											  kCFSocketConnectCallBack, cfCallBack, &sockContext))) {
		return nil;
	}
	
	////////////////////////////设置地址///////////////////	
	NSString *serverAddr = ip;
	struct   sockaddr_in  addr;
	memset(&addr , 0,sizeof(addr));
	addr.sin_len = sizeof(addr);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr([serverAddr  UTF8String]);
	
    CFDataRef address = CFDataCreate(kCFAllocatorDefault,(UInt8*)&addr,sizeof(addr));
	
	/////////////////////////////执行连接/////////////////////
    CFSocketConnectToAddress(cfSocket,address,-1);
	// add to run loop
	CFRunLoopSourceRef source;
	if (!(source = CFSocketCreateRunLoopSource(NULL, cfSocket, 0))) {
		return nil;
	}
	CFRunLoopAddSource(CFRunLoopGetCurrent(), source, kCFRunLoopDefaultMode);
	CFRelease(source);
	
	return self;
}

- (void) sendMessage:(NSString *)msg
{
	const char *data = [msg UTF8String];
	send(CFSocketGetNative(cfSocket), data, strlen(data) + 1, 0);
	
}

- (void) sendMessage:(char *)msg andLen:(int)msgLen
{
	send(CFSocketGetNative(cfSocket), msg, msgLen + 1, 0);
	
}

- (void) closeConnect
{
	close(CFSocketGetNative(cfSocket));
	bConnected=[NSNumber numberWithBool:false];
	
}
@end
