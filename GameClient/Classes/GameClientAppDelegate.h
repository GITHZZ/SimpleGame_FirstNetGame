//
//  GameClientAppDelegate.h
//  GameClient
//
//  Created by studentg07 on 12-4-7.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@class GameClientViewController;

@interface GameClientAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    GameClientViewController *viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet GameClientViewController *viewController;

@end

