//
//  myUIImageView.h
//  GameClient
//
//  Created by studentg07 on 12-4-8.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#define card_UNVISIBLE 0//不可见
#define card_NORMAL    1//正常可点击
#define card_SELECT    2//点击选中状态
#define card_ONLYSEE   3//仅仅可见但不能选或点击


@interface myUIImageView:UIImageView{
	UIImageView *theImageView;
	NSNumber *cardData;     //牌的值
	NSNumber *cardNumber;   //牌的编号
	int cardState;    //牌的状态
	CGRect cardRect; //牌的位置及大小
}
@property (nonatomic, retain) UIImageView *theImageView; 
@property (nonatomic, retain) NSNumber *cardData;
@property (nonatomic, retain) NSNumber *cardNumber;

-(void)onClickImage;
-(void)initMyUIImageView:(NSNumber *)iNumber andData:(NSNumber *)iData andState:(int)iState; 
-(BOOL)sendCardInScreen;

@end
