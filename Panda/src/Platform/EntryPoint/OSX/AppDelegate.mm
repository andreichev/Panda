//
//  AppDelegate.m
//
//  Created by Andreichev Mikhail on 07.02.2025.
//

#import "AppDelegate.hpp"

@implementation AppDelegate {
    id keyUpMonitor;
}

- (void)applicationDidFinishLaunching:(NSNotification *)notification {
    NSEvent* event = [NSEvent otherEventWithType:NSEventTypeApplicationDefined
                                        location:NSMakePoint(0, 0)
                                   modifierFlags:0
                                       timestamp:0
                                    windowNumber:0
                                         context:nil
                                         subtype:0
                                           data1:0
                                           data2:0];
    [NSApp postEvent:event atStart:YES];
    NSLog(@"DID FINISH LAUNCHING");
    NSEvent* (^block)(NSEvent*) = ^ NSEvent* (NSEvent* event) {
        if ([event modifierFlags] & NSEventModifierFlagCommand) {
            [[NSApp keyWindow] sendEvent:event];
        }
        return event;
    };
    keyUpMonitor = [NSEvent addLocalMonitorForEventsMatchingMask:NSEventMaskKeyUp
                                                         handler:block];
}

- (void)applicationWillTerminate:(NSNotification *)notification {
    NSLog(@"APP WILL TERMINATE");
    if (keyUpMonitor) {
        [NSEvent removeMonitor:keyUpMonitor];
    }
}

- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication *)sender {
    return NSTerminateCancel;
}

@end
