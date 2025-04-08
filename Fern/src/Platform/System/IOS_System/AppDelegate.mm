//
//  AppDelegate.m
//
//  Created by Andreichev Mikhail on 07.02.2025.
//

#import "AppDelegate.h"
#import "UIKitWindow/WonderController.h"

#import <filesystem>

extern int startApp(int argc, char** argv);

std::filesystem::path g_ios_resources_path;

@implementation WonderAppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    WonderController* controller = [[WonderController alloc] init];
    _window = [[UIWindow alloc] initWithFrame:UIScreen.mainScreen.bounds];

    NSString* path = NSBundle.mainBundle.resourcePath;
    g_ios_resources_path = path.UTF8String;
    NSLog(@"Resources path: %@", path);

    _window.rootViewController = controller;
    [_window makeKeyAndVisible];

    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        startApp(0, nullptr);
    });

    return YES;
}

@end
