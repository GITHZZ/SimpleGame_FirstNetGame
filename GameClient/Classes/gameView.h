//
//  gameView.h
//  GameClient
//
//  Created by studentg07 on 12-4-7.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#include "MsgDefine.h"
#import "CFSocketClient.h"
#import "myUIImageView.h"
#import "GameClientViewController.h"
#import "UIViewPassValueDelegate.h"

@class GameClientViewController;

@interface gameView : UIViewController<UITextViewDelegate> {
	UITextField *inputMsg; 
	UIButton *btnSend;
	UITextView *textView ;
	CFSocketClient *myClient;
	myUIImageView  *myImageViewList[MAX_CARD_COUNT]; //自己的牌
	
	GameClientViewController *theGameClientView;
	PLAYER_INFO playerList[MAX_GAME_PLAYER];
	int playerCount;     //当前游戏玩家个数
	int myID;//当前玩家的ID
	UIButton *btnReady;
	UIButton *btnSendCard;
	UIButton *btnDouble;
	
	UILabel *cardNumLabel;
	
	BOOL isPlaying;
	int gameState;
	int cardNum[10];
}
@property(nonatomic, retain) IBOutlet UIButton *btnReady;

@property (nonatomic, retain) IBOutlet UITextField *inputMsg;  
@property (nonatomic, retain) IBOutlet UIButton *btnSend; 
@property (nonatomic, retain) IBOutlet UIButton *btnSendCard;
@property (nonatomic, retain) IBOutlet UIButton *btnDouble;
@property (nonatomic, retain) IBOutlet UITextView *textView; 

@property (nonatomic, retain) IBOutlet GameClientViewController *theGameClientView;
@property (nonatomic, retain) IBOutlet UIImageView *theImageView;

@property (nonatomic, retain) IBOutlet UILabel *cardNumLabel;

- (IBAction) textFieldDoneEditing:(id)sender;  
- (IBAction) backgroundTouch:(id)sender; 
- (IBAction) sendMsg; 

- (IBAction) returnLogin; 
- (IBAction) sendReady;
- (IBAction) sendCard;
- (IBAction) doubleButton;

- (void) showTextView:(NSString *)str;
- (void) initGameView:(CFSocketClient*) client;
- (void) addOnePlayer:(PLAYER_INFO*)player;  //新用户加入游戏
- (void) addAllPlayer:(MSG_ALLPLAYER*)allplayer;//自己加入游戏
- (void) getNewCard:(MSG_NEWCARD*)theNewCard;


@end
