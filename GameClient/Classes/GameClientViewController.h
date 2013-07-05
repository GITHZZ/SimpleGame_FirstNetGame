//
//  GameClientViewController.h
//  GameClient
//
//  Created by studentg07 on 12-4-7.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "CFSocketClient.h"
#import "MsgDefine.h"
#import "gameView.h"

@class gameView;

@interface GameClientViewController : UIViewController <CFSocketDelegate> {
	CFSocketClient *myClient;
	
	UITextField *txtName;
	UITextField *txtPass;
	UITextField *txtIpAddress;
	UIButton *btnLogin;
	UIButton *btnLeave;
	gameView *theGameView;
	
	int myGameState;//游戏状态
	
}

@property (nonatomic,retain) IBOutlet gameView *theGameView;

@property (nonatomic, retain) IBOutlet UITextField *txtName;
@property (nonatomic, retain) IBOutlet UITextField *txtPass;
@property (nonatomic, retain) IBOutlet UITextField *txtIpAddress;
@property (nonatomic, retain) IBOutlet UIButton *btnLogin; 
@property (nonatomic, retain) IBOutlet UIButton *btnLeave;

@property int myGameState;
		   
- (void) changeToGameView;

- (IBAction) userLogin;
- (IBAction) userLeave;
@end

