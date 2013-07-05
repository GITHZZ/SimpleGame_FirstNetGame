//
//  myUIImageView.m
//  GameClient
//
//  Created by studentg07 on 12-4-8.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import "myUIImageView.h"

@implementation myUIImageView
@synthesize theImageView;
@synthesize cardData;
@synthesize cardNumber;

-(void)initMyUIImageView:(NSNumber *)iNumber andData:(NSNumber *)iData andState:(int)iState
{
	cardNumber=iNumber;
	cardData=iData;
	cardRect=CGRectMake(30.0*[iNumber intValue],150.0,64,108);
	theImageView =[[UIImageView alloc]initWithFrame:cardRect];
	theImageView.contentMode = UIViewContentModeScaleAspectFill; 
	NSString *imageName=[NSString stringWithFormat:@"%d.png",iData];
	theImageView.image = [UIImage imageNamed:imageName];//加载入图片
	
	cardState=iState;
	if (cardState!=card_ONLYSEE) {
		theImageView.userInteractionEnabled=YES;
		UITapGestureRecognizer* singleTap =[[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(onClickImage)];
		[theImageView addGestureRecognizer:singleTap];
		[singleTap release];
	}
}

-(void)onClickImage
{
	if (cardState==card_NORMAL) {
		[theImageView setFrame:CGRectOffset(cardRect, 0, -10.0)];
		[theImageView setAlpha:0.8];
		cardState=card_SELECT;
	}
	else if(cardState==card_SELECT) {
		[theImageView setFrame:cardRect];
		[theImageView setAlpha:1];
		cardState=card_NORMAL;
	}
}

-(BOOL)sendCardInScreen
{
	if (cardState==card_SELECT) 
	{
		cardRect.origin=CGPointMake(150, 50);
		cardState=card_ONLYSEE;
		[theImageView setFrame:cardRect];
		return YES;
	}
	
	return NO;
}

@end
