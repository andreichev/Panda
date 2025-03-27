//
//  main.mm
//
//  Created by Andreichev Mikhail on 07.02.2025.
//

#import <Cocoa/Cocoa.h>

#import "AppDelegate.h"

static void changeToResourcesDirectory() {
    char resourcesPath[MAXPATHLEN];
    CFBundleRef bundle = CFBundleGetMainBundle();
    if (!bundle) {
        return;
    }
    CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(bundle);
    CFStringRef last = CFURLCopyLastPathComponent(resourcesURL);
    if (CFStringCompare(CFSTR("Resources"), last, 0) != kCFCompareEqualTo) {
        CFRelease(last);
        CFRelease(resourcesURL);
        return;
    }
    CFRelease(last);
    if (!CFURLGetFileSystemRepresentation(resourcesURL,
                                          true,
                                          (UInt8*) resourcesPath,
                                          MAXPATHLEN)) {
        CFRelease(resourcesURL);
        return;
    }
    CFRelease(resourcesURL);
    chdir(resourcesPath);
}

extern int startApp(int argc, char** argv);

@interface MainHelper : NSObject
@end

@implementation MainHelper
- (void)selectedKeyboardInputSourceChanged:(NSObject* )object {}
- (void)doNothing:(id)object{}
@end

int main(int argc, const char * argv[]) {
    MainHelper* helper = [[MainHelper alloc] init];
    [NSThread detachNewThreadSelector:@selector(doNothing:)
                             toTarget:helper
                           withObject:nil];
    NSApplication* application = [NSApplication sharedApplication];
    AppDelegate* delegate = [[AppDelegate alloc] init];
    [application setDelegate:delegate];
    [application setActivationPolicy:NSApplicationActivationPolicyRegular];
    [application activateIgnoringOtherApps:YES];
    changeToResourcesDirectory();
    [application finishLaunching];
    id quitMenuItem = [NSMenuItem new];
    [quitMenuItem
        initWithTitle:@"Quit"
        action:@selector(terminate:)
        keyEquivalent:@"q"];
    id appMenu = [NSMenu new];
    [appMenu addItem:quitMenuItem];
    id appMenuItem = [NSMenuItem new];
    [appMenuItem setSubmenu:appMenu];
    id menubar = [[NSMenu new] autorelease];
    [menubar addItem:appMenuItem];
    [application setMainMenu:menubar];
    [application run];
    int result = startApp(0, nullptr);
    [application terminate:nil];
    [helper release];
    return result;
}
