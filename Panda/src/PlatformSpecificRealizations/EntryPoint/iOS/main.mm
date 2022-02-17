//
//  main.m
//  Panda
//
//  Created by Admin on 03.08.2021.
//

#include "PlatformSpecificRealizations/WindowImpl/UIKitWindow/Controller/WonderController.hpp"
#import <UIKit/UIKit.h>

extern int startApp(int argc, char** argv);

@interface AppDelegate : UIResponder <UIApplicationDelegate>

@property (strong, nonatomic) UIWindow *window;

@end

@implementation AppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    WonderController* controller = [[WonderController alloc] init];
    _window = [[UIWindow alloc] initWithFrame:UIScreen.mainScreen.bounds];

    _window.rootViewController = controller;
    [_window makeKeyAndVisible];

    startApp(0, nullptr);

    return YES;
}

@end

int main(int argc, char * argv[]) {
    @autoreleasepool {
        return UIApplicationMain(argc, argv, nil, NSStringFromClass([AppDelegate class]));
    }
}
