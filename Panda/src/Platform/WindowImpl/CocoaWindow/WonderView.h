//
//  GameViewController.h
//
//  Created by Andreichev Mikhail on 07.02.2025.
//

#import <Cocoa/Cocoa.h>
#import <Panda/Events/EventQueue.hpp>

@interface WonderView : NSView {
    Panda::EventQueue* eventQueue;
}

- (void)setEventQueue:(Panda::EventQueue*)queue;

@end
