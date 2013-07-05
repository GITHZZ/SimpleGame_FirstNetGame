//
//  gameView.m
//  GameClient
//
//  Created by studentg07 on 12-4-7.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import "gameView.h"

@implementation gameView
@synthesize btnReady;
@synthesize btnSendCard;
@synthesize btnDouble;
@synthesize inputMsg, btnSend ,textView;
@synthesize theImageView;
@synthesize theGameClientView;

@synthesize cardNumLabel;

- (void) getNewCard:(MSG_NEWCARD*)theNewCard
{
	for (int i=0; i<MAX_CARD_COUNT;i++) {
		NSString* imageName=[NSString stringWithFormat:@"%d.png",
							 theNewCard->CardList[i]];
		cardNum[i]=theNewCard->CardList[i];
		myImageViewList[i].theImageView.image =[UIImage imageNamed:imageName];
		
	}
	
	isPlaying=YES;
}


- (IBAction) sendReady
{
	btnReady.hidden=YES;
	MSG_READY theReady;
	theReady.nType=MSGID_READY;
	theReady.nLen=sizeof(struct MSG_READY);
	theReady.nID=myID;
	char* p=(char*)&theReady;
	[myClient sendMessage:p andLen:theReady.nLen];
	
}

- (void) addOnePlayer:(PLAYER_INFO*)player
{
	memcpy((void*)&playerList[playerCount],(void*)&player,sizeof(struct PLAYER_INFO));
	playerCount++;
}

- (void) addAllPlayer:(MSG_ALLPLAYER*)allplayer
{
	for(int i=0;i<=allplayer->playerCount;i++)
	{
		memcpy((void*)&playerList[i],(void*)&allplayer->playerList[i],sizeof(struct PLAYER_INFO));
		playerCount++;
	}
	myID=allplayer->playerCount;
	
}


- (IBAction) sendMsg{  
	if (inputMsg.text!=NULL) {
		NSString *inputMsgStr = self.inputMsg.text;  
		NSLog(@"%@",inputMsgStr); 
		
		MSG_CHAT myChat;
		myChat.nType=MSGID_CHAT;
		myChat.nLen=sizeof(struct MSG_CHAT);
		strcpy(myChat.Chat,[inputMsgStr  UTF8String]);
		char *p=(char*)&myChat;
		[myClient sendMessage:p andLen:myChat.nLen];
		
		inputMsg.text=NULL;
		
	}

}  

- (IBAction) returnLogin
{
	[myClient closeConnect];
	[self presentModalViewController:theGameClientView animated:YES];
}

- (IBAction) sendCard
{
	MSG_PUTCARD putCard;
	int count=0;
	int temp=0;
	
	if (isPlaying) 
	{
		for (int i=0; i<MAX_CARD_COUNT;i++) 
		{
			if ([myImageViewList[i] sendCardInScreen]) 
			{
				count++;
				putCard.CardList=cardNum[i]%13;
				temp++;
				break;
			}
		}
		 
		putCard.nType=MSGID_PUTCARD;
		putCard.nLen=sizeof(struct MSG_PUTCARD);
		putCard.nID=myID;
		putCard.game_Count=gameState;
		putCard.nCount=count;
		char* p=(char*)&putCard;
		[myClient sendMessage:p andLen:putCard.nLen];
		gameState=-gameState;
	}
	
}

- (IBAction) doubleButton{
	MSG_GAMEDOUBLE cardDouble;
	cardDouble.nType=MSGID_GAMEDOUBLE;
	cardDouble.nLen=sizeof(struct MSG_GAMEDOUBLE);
	char *p=(char*)&cardDouble;
	[myClient sendMessage:p andLen:cardDouble.nLen];
}

- (IBAction) textFieldDoneEditing:(id)sender{ 
    [inputMsg resignFirstResponder];  
		
}  

- (IBAction) backgroundTouch:(id)sender{  
    [inputMsg resignFirstResponder];  
}  


- (void) initGameView:(CFSocketClient*) client
{
	isPlaying=NO;
	myClient=client;
	theGameClientView=[GameClientViewController alloc];
	self.textView.delegate = self;//设置它的委托方法
	
	for (int i=0; i<MAX_CARD_COUNT; i++) 
	{
		myImageViewList[i]=[myUIImageView alloc];
		NSNumber *num=[NSNumber numberWithInt:i];
		NSNumber *numData=[NSNumber numberWithInt:55];
		
		[myImageViewList[i] initMyUIImageView:num andData:numData andState:1];
		 
		[self.view addSubview:[myImageViewList[i] theImageView]];
		
	}	

}
// The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
/*
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization.
    }
    return self;
}
*/


// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad 
{
    [super viewDidLoad];
	btnSendCard.hidden=YES;
	gameState=1;
}


/*
// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations.
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}
*/

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc. that aren't in use.
}

- (void)viewDidUnload {
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}


- (void)dealloc {
	
	[super dealloc];
}

- (void) showTextView:(NSString *)str
{
	NSString *str1=[[NSString alloc] initWithString:textView.text];
	NSString *str2=[[NSString alloc] initWithFormat:@"%@\n%@",str1,str];
	textView.text=str2;
	[textView scrollRangeToVisible:NSMakeRange(textView.text.length - 1, 0)];
	[str1 release];
	[str2 release];
}


@end
