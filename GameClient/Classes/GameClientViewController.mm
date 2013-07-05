//
//  GameClientViewController.m
//  GameClient
//
//  Created by studentg07 on 12-4-7.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import "GameClientViewController.h"
@implementation GameClientViewController

@synthesize txtName,txtPass,txtIpAddress,btnLogin,btnLeave;
@synthesize theGameView;
@synthesize myGameState;

- (void) changeToGameView{
	[self presentModalViewController:theGameView animated:YES];

}


/*
 // The designated initializer. Override to perform setup that is required before the view is loaded.
 - (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
 self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
 if (self) {
 // Custom initialization
 }
 return self;
 }
 */

/*
 // Implement loadView to create a view hierarchy programmatically, without using a nib.
 - (void)loadView {

 }
*/

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
	
}

-(IBAction) userLogin{
	if(![myClient.bConnected boolValue] )
	{
		NSString *address=self.txtIpAddress.text;
		myClient =[[CFSocketClient alloc] connectServer:address andPort:8000];
		myClient.delegate = self;
	}
	else {
	  //  NSString *tmp=[NSString stringWithFormat:@"你已经连接了服务器，不能重复登录！"];
	  //  [self showTextView:tmp];
	}
	
}

- (IBAction) userLeave
{
	[myClient closeConnect];
	
}

/*
 // Override to allow orientations other than the default portrait orientation.
 - (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
 // Return YES for supported orientations
 return (interfaceOrientation == UIInterfaceOrientationPortrait);
 }
 */

- (void)onConnect:(const void *)data
{
	if (data) { // connect failed
		printf("连接失败\n");
		myClient.bConnected=[NSNumber numberWithBool:false];
	} else {
		printf("连接成功\n");
		NSString *name=self.txtName.text;
		NSString *pwd=self.txtPass.text;
		
		MSG_LOGIN myLogin;
		myLogin.nType=MSGID_LOGIN;
		myLogin.nLen=sizeof(struct MSG_LOGIN);
		strcpy(myLogin.Name,[name  UTF8String]);
		strcpy(myLogin.Pwd,[pwd UTF8String]);
		char *p=(char*)&myLogin;
		[myClient sendMessage:p andLen:myLogin.nLen];
		myClient.bConnected=[NSNumber numberWithBool:true];
		[theGameView initGameView:myClient];
		[self changeToGameView];
		
	}
	
}

- (void)onWrite:(const void *)data
{
	
	printf("触发Write消息\n");
	
}

- (void)onRead:(char *)recvbuf andLen:(int)n
{

	MSG_GENERIC* pGenericMsg = (MSG_GENERIC*)recvbuf;

	switch(pGenericMsg->nType) // 判断收到的消息类型
	{
		case MSGID_NEWCARD:
		{
			MSG_NEWCARD* pNew=(MSG_NEWCARD*)recvbuf;
			[theGameView getNewCard:pNew];
			if (pNew->is_MainPlayper!=1) 
			{
				theGameView.btnSendCard.hidden=NO;
			}
			else {
				theGameView.btnSendCard.hidden=YES;
			}

		}
			break;
		case MSGID_CHAT:
		{
			MSG_CHAT* pChat=(MSG_CHAT*)recvbuf;
			
			[theGameView showTextView:[NSString stringWithUTF8String:pChat->Chat]];
			
			//myGameState=pChat->gameState;
//			NSLog(@"%d",myGameState);
			
		}
			break;
		case MSGID_NEWPLAYER:
		{
			MSG_NEWPLAYER *pNew=(MSG_NEWPLAYER*)recvbuf;
			
			NSString *tmp=[NSString stringWithFormat:@"%@登录了游戏！",
						   [NSString stringWithUTF8String:pNew->player.Name]];
			
			[theGameView showTextView:tmp];
			[theGameView addOnePlayer:&pNew->player];
		}
			break;
			
		case MSGID_ALLPLAYER:
		{
			MSG_ALLPLAYER *pAll=(MSG_ALLPLAYER*)recvbuf;
			[theGameView addAllPlayer:pAll];
		}
			break;
			
		case MSGID_GAMENUM:
		{
			MSG_GAMENUM *pNum=(MSG_GAMENUM *) recvbuf;
			NSString *tempStr=[NSString stringWithFormat:@"%d",pNum->cardNum];
			theGameView.cardNumLabel.text=tempStr;
			
			if (pNum->game_Count) 
			{
				theGameView.btnSendCard.hidden=YES;
			}
			else 
			{
				theGameView.btnSendCard.hidden=NO;
			}
			
		}
			break;
	}

}


- (void)didReceiveMemoryWarning {
	// Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
	
	// Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
	// Release any retained subviews of the main view.
	// e.g. self.myOutlet = nil;
}


- (void)dealloc {
	[theGameView release];
    [super dealloc];
}

@end
