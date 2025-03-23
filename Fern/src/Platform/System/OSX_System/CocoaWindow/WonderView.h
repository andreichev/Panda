//
//  GameViewController.h
//
//  Created by Andreichev Mikhail on 07.02.2025.
//

#import <Cocoa/Cocoa.h>

@interface WonderView : NSView<NSTextInputClient> {
    NSMutableAttributedString* markedText;
}

@end
