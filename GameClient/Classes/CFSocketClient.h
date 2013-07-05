//
//  CFSocketClient.h
//  Lab6CFNetWorkClient
//
//  Created by studentg07 on 12-3-25.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CFNetwork/CFNetwork.h>
#import <sys/socket.h>
#import <netinet/in.h>
#import <arpa/inet.h>
#import <netdb.h>
@class CFSocketClient;

@protocol CFSocketDelegate
- (void)onConnect:(const void *)data;
- (void)onRead:(char *)recvbuf andLen:(int)n;
- (void)onWrite:(const void *)data;
@end

static void cfCallBack(CFSocketRef cf, CFSocketCallBackType type, 
					   CFDataRef addr, const void *data, void *info);
@interface CFSocketClient : NSObject {
	CFSocketRef cfSocket;
	NSObject<CFSocketDelegate> * delegate;
	NSNumber *bConnected;

}
@property(nonatomic, retain) NSObject<CFSocketDelegate> * delegate;
@property (nonatomic, retain)  NSNumber *bConnected;
- (id)connectServer;
- (id)connectServer:(NSString *)ip andPort:(int)port;
- (void) sendMessage:(NSString *)msg;
- (void) sendMessage:(char *)msg andLen:(int)msgLen;
- (void) closeConnect;

@end
