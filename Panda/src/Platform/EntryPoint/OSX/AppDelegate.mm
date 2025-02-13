//
//  AppDelegate.m
//
//  Created by Andreichev Mikhail on 07.02.2025.
//

#import "AppDelegate.hpp"

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)notification {
    NSLog(@"DID FINISH LAUNCHING");
}

- (void)applicationWillTerminate:(NSNotification *)notification {
    NSLog(@"APP WILL TERMINATE");
}

- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication *)sender {
    return NSTerminateCancel;
}

@end
