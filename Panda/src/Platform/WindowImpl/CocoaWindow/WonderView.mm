//
//  WonderView.mm
//
//  Created by Andreichev Mikhail on 07.02.2025.
//

#import "WonderView.h"

using namespace Panda;

struct WonderHelper {
    Panda::Key keycodes[256];
    Panda::Key scancodes[256];
    bool keys[256];
};

static const NSRange kEmptyRange = { NSNotFound, 0 };

static void fillKeyTable(WonderHelper& helper) {
    memset(helper.keycodes, -1, sizeof(helper.keycodes));
    memset(helper.scancodes, -1, sizeof(helper.scancodes));

    helper.keycodes[0x1D] = Panda::Key::KEY_0;
    helper.keycodes[0x12] = Panda::Key::KEY_1;
    helper.keycodes[0x13] = Panda::Key::KEY_2;
    helper.keycodes[0x14] = Panda::Key::KEY_3;
    helper.keycodes[0x15] = Panda::Key::KEY_4;
    helper.keycodes[0x17] = Panda::Key::KEY_5;
    helper.keycodes[0x16] = Panda::Key::KEY_6;
    helper.keycodes[0x1A] = Panda::Key::KEY_7;
    helper.keycodes[0x1C] = Panda::Key::KEY_8;
    helper.keycodes[0x19] = Panda::Key::KEY_9;
    helper.keycodes[0x00] = Panda::Key::A;
    helper.keycodes[0x0B] = Panda::Key::B;
    helper.keycodes[0x08] = Panda::Key::C;
    helper.keycodes[0x02] = Panda::Key::D;
    helper.keycodes[0x0E] = Panda::Key::E;
    helper.keycodes[0x03] = Panda::Key::F;
    helper.keycodes[0x05] = Panda::Key::G;
    helper.keycodes[0x04] = Panda::Key::H;
    helper.keycodes[0x22] = Panda::Key::I;
    helper.keycodes[0x26] = Panda::Key::J;
    helper.keycodes[0x28] = Panda::Key::K;
    helper.keycodes[0x25] = Panda::Key::L;
    helper.keycodes[0x2E] = Panda::Key::M;
    helper.keycodes[0x2D] = Panda::Key::N;
    helper.keycodes[0x1F] = Panda::Key::O;
    helper.keycodes[0x23] = Panda::Key::P;
    helper.keycodes[0x0C] = Panda::Key::Q;
    helper.keycodes[0x0F] = Panda::Key::R;
    helper.keycodes[0x01] = Panda::Key::S;
    helper.keycodes[0x11] = Panda::Key::T;
    helper.keycodes[0x20] = Panda::Key::U;
    helper.keycodes[0x09] = Panda::Key::V;
    helper.keycodes[0x0D] = Panda::Key::W;
    helper.keycodes[0x07] = Panda::Key::X;
    helper.keycodes[0x10] = Panda::Key::Y;
    helper.keycodes[0x06] = Panda::Key::Z;

    helper.keycodes[0x27] = Panda::Key::APOSTROPHE;
    helper.keycodes[0x2A] = Panda::Key::BACKSLASH;
    helper.keycodes[0x2B] = Panda::Key::COMMA;
    helper.keycodes[0x18] = Panda::Key::EQUAL;
    helper.keycodes[0x32] = Panda::Key::GRAVE_ACCENT;
    helper.keycodes[0x21] = Panda::Key::LEFT_BRACKET;
    helper.keycodes[0x1B] = Panda::Key::MINUS;
    helper.keycodes[0x2F] = Panda::Key::PERIOD;
    helper.keycodes[0x1E] = Panda::Key::RIGHT_BRACKET;
    helper.keycodes[0x29] = Panda::Key::SEMICOLON;
    helper.keycodes[0x2C] = Panda::Key::SLASH;
    helper.keycodes[0x0A] = Panda::Key::WORLD_1;

    helper.keycodes[0x33] = Panda::Key::BACKSPACE;
    helper.keycodes[0x39] = Panda::Key::CAPS_LOCK;
    helper.keycodes[0x75] = Panda::Key::DELETE;
    helper.keycodes[0x7D] = Panda::Key::DOWN;
    helper.keycodes[0x77] = Panda::Key::END;
    helper.keycodes[0x24] = Panda::Key::ENTER;
    helper.keycodes[0x35] = Panda::Key::ESCAPE;
    helper.keycodes[0x7A] = Panda::Key::F1;
    helper.keycodes[0x78] = Panda::Key::F2;
    helper.keycodes[0x63] = Panda::Key::F3;
    helper.keycodes[0x76] = Panda::Key::F4;
    helper.keycodes[0x60] = Panda::Key::F5;
    helper.keycodes[0x61] = Panda::Key::F6;
    helper.keycodes[0x62] = Panda::Key::F7;
    helper.keycodes[0x64] = Panda::Key::F8;
    helper.keycodes[0x65] = Panda::Key::F9;
    helper.keycodes[0x6D] = Panda::Key::F10;
    helper.keycodes[0x67] = Panda::Key::F11;
    helper.keycodes[0x6F] = Panda::Key::F12;
    helper.keycodes[0x69] = Panda::Key::PRINT_SCREEN;
    helper.keycodes[0x6B] = Panda::Key::F14;
    helper.keycodes[0x71] = Panda::Key::F15;
    helper.keycodes[0x6A] = Panda::Key::F16;
    helper.keycodes[0x40] = Panda::Key::F17;
    helper.keycodes[0x4F] = Panda::Key::F18;
    helper.keycodes[0x50] = Panda::Key::F19;
    helper.keycodes[0x5A] = Panda::Key::F20;
    helper.keycodes[0x73] = Panda::Key::HOME;
    helper.keycodes[0x72] = Panda::Key::INSERT;
    helper.keycodes[0x7B] = Panda::Key::LEFT;
    helper.keycodes[0x3A] = Panda::Key::LEFT_ALT;
    helper.keycodes[0x3B] = Panda::Key::LEFT_CONTROL;
    helper.keycodes[0x38] = Panda::Key::LEFT_SHIFT;
    helper.keycodes[0x37] = Panda::Key::LEFT_SUPER;
    helper.keycodes[0x6E] = Panda::Key::MENU;
    helper.keycodes[0x47] = Panda::Key::NUM_LOCK;
    helper.keycodes[0x79] = Panda::Key::PAGE_DOWN;
    helper.keycodes[0x74] = Panda::Key::PAGE_UP;
    helper.keycodes[0x7C] = Panda::Key::RIGHT;
    helper.keycodes[0x3D] = Panda::Key::RIGHT_ALT;
    helper.keycodes[0x3E] = Panda::Key::RIGHT_CONTROL;
    helper.keycodes[0x3C] = Panda::Key::RIGHT_SHIFT;
    helper.keycodes[0x36] = Panda::Key::RIGHT_SUPER;
    helper.keycodes[0x31] = Panda::Key::SPACE;
    helper.keycodes[0x30] = Panda::Key::TAB;
    helper.keycodes[0x7E] = Panda::Key::UP;

    helper.keycodes[0x52] = Panda::Key::KP_0;
    helper.keycodes[0x53] = Panda::Key::KP_1;
    helper.keycodes[0x54] = Panda::Key::KP_2;
    helper.keycodes[0x55] = Panda::Key::KP_3;
    helper.keycodes[0x56] = Panda::Key::KP_4;
    helper.keycodes[0x57] = Panda::Key::KP_5;
    helper.keycodes[0x58] = Panda::Key::KP_6;
    helper.keycodes[0x59] = Panda::Key::KP_7;
    helper.keycodes[0x5B] = Panda::Key::KP_8;
    helper.keycodes[0x5C] = Panda::Key::KP_9;
    helper.keycodes[0x45] = Panda::Key::KP_ADD;
    helper.keycodes[0x41] = Panda::Key::KP_DECIMAL;
    helper.keycodes[0x4B] = Panda::Key::KP_DIVIDE;
    helper.keycodes[0x4C] = Panda::Key::KP_ENTER;
    helper.keycodes[0x51] = Panda::Key::KP_EQUAL;
    helper.keycodes[0x43] = Panda::Key::KP_MULTIPLY;
    helper.keycodes[0x4E] = Panda::Key::KP_SUBTRACT;

    for (int scancode = 0;  scancode < 256;  scancode++) {
        if ((int)helper.keycodes[scancode] >= 0)
            helper.scancodes[(int)helper.keycodes[scancode]] = (Panda::Key)scancode;
    }
}

static Panda::Key cocoaKeyToPandaKey(WonderHelper& helper, unsigned int key) {
    if (key >= sizeof(helper.keycodes) / sizeof(helper.keycodes[0])) {
        return Panda::Key::UNKNOWN;
    }
    return helper.keycodes[key];
}

static NSUInteger pandaKeyToCocoaModifierFlag(Panda::Key key) {
    switch (key) {
        case Panda::Key::LEFT_SHIFT:
        case Panda::Key::RIGHT_SHIFT:
            return NSEventModifierFlagShift;
        case Panda::Key::LEFT_CONTROL:
        case Panda::Key::RIGHT_CONTROL:
            return NSEventModifierFlagControl;
        case Panda::Key::LEFT_ALT:
        case Panda::Key::RIGHT_ALT:
            return NSEventModifierFlagOption;
        case Panda::Key::LEFT_SUPER:
        case Panda::Key::RIGHT_SUPER:
            return NSEventModifierFlagCommand;
        case Panda::Key::CAPS_LOCK:
            return NSEventModifierFlagCapsLock;
    }
    return 0;
}

@implementation WonderView {
    WonderHelper m_helper;
}

- (instancetype)init {
    self = [super init];
    if (self != nil) {
        eventQueue = nil;
        memset(m_helper.keys, 0, sizeof(m_helper.keys));
        markedText = [[NSMutableAttributedString alloc] init];
        fillKeyTable(m_helper);
    }
    NSLog(@"WonderView created");
    return self;
}

- (void)dealloc {
    [super dealloc];
    [markedText release];
    NSLog(@"WonderView deallocated");
}

- (void)setEventQueue:(Panda::EventQueue *)queue {
    eventQueue = queue;
}

- (BOOL)acceptsFirstResponder {
    return YES;
}

- (BOOL)canBecomeKeyView {
    return YES;
}

- (BOOL)wantsUpdateLayer {
    return YES;
}

// MARK: - Touchpad

- (void)touchesBeganWithEvent:(NSEvent *)event {
    if (eventQueue == nil) { return; }
    NSSet<NSTouch *>* touches = [event touchesMatchingPhase:NSTouchPhaseBegan inView:self];
    for (NSTouch *touch in touches) {
        CGPoint touchLocation = [touch normalizedPosition];
        // LOG_INFO("TOUCH BEGAN: {}", int([touch.identity hash]));
        float x = touchLocation.x * self.frame.size.width;
        float y = (1.f - touchLocation.y) * self.frame.size.height;
        eventQueue->postTouchBeganEvent(int([touch.identity hash]), x, y);
    }
}

- (void)touchesMovedWithEvent:(NSEvent *)event {
    if (eventQueue == nil) { return; }
    NSSet<NSTouch *>* touches = [event touchesMatchingPhase:NSTouchPhaseMoved inView:self];
    for (NSTouch *touch in touches) {
        CGPoint touchLocation = [touch normalizedPosition];
        // LOG_INFO("TOUCH MOVED: {}", int([touch.identity hash]));
        float x = touchLocation.x * self.frame.size.width;
        float y = (1.f - touchLocation.y) * self.frame.size.height;
        eventQueue->postTouchMovedEvent(int([touch.identity hash]), x, y);
    }
}

- (void)touchesEndedWithEvent:(NSEvent *)event {
    if (eventQueue == nil) { return; }
    NSSet<NSTouch *>* touches = [event touchesMatchingPhase:NSTouchPhaseEnded inView:self];
    for (NSTouch *touch in touches) {
        // LOG_INFO("TOUCH ENDED: {}", int([touch.identity hash]));
        eventQueue->postTouchEndedEvent(int([touch.identity hash]));
    }
}

// MARK: - Mouse

- (void)mouseMoved:(NSEvent *)event {
    if (eventQueue == nil) { return; }
    // NOTE: The returned location uses base 0,1 not 0,0
    const NSPoint pos = [event locationInWindow];
    double xpos = pos.x;
    double ypos = self.frame.size.height - pos.y;
    eventQueue->postMouseEvent(xpos, ypos);
}

- (void)mouseDragged:(NSEvent *)event {
    [self mouseMoved:event];
}

- (void)rightMouseDragged:(NSEvent *)event {
    [self mouseMoved:event];
}

- (void)mouseDown:(NSEvent *)event {
    if (eventQueue == nil) { return; }
    // Command + Left Mouse Button acts as middle! This just a temporary solution!
    // This is because the average OSX user doesn't have middle mouse click.
    MouseButton mb = ([event modifierFlags] & NSEventModifierFlagCommand)
    ? MouseButton::MIDDLE
    : MouseButton::LEFT
    ;
    eventQueue->postMouseButtonEvent(mb, true);
}

- (void)mouseUp:(NSEvent *)event {
    if (eventQueue == nil) { return; }
    MouseButton mb = ([event modifierFlags] & NSEventModifierFlagCommand)
    ? MouseButton::MIDDLE
    : MouseButton::LEFT
    ;
    eventQueue->postMouseButtonEvent(mb, false);
}

- (void)rightMouseDown:(NSEvent *)event {
    if (eventQueue == nil) { return; }
    eventQueue->postMouseButtonEvent(MouseButton::RIGHT, true);
}

- (void)rightMouseUp:(NSEvent *)event {
    if (eventQueue == nil) { return; }
    eventQueue->postMouseButtonEvent(MouseButton::RIGHT, false);
}

- (void)otherMouseDown:(NSEvent *)event {
    if (eventQueue == nil) { return; }
    eventQueue->postMouseButtonEvent(MouseButton::MIDDLE, true);
}

- (void)otherMouseUp:(NSEvent *)event {
    if (eventQueue == nil) { return; }
    eventQueue->postMouseButtonEvent(MouseButton::RIGHT, false);
}

- (void)scrollWheel:(NSEvent *)event {
    if (eventQueue == nil) { return; }
    double deltaX = [event scrollingDeltaX];
    double deltaY = [event scrollingDeltaY];
    if ([event hasPreciseScrollingDeltas]) {
        deltaX *= 0.1;
        deltaY *= 0.1;
    }
    eventQueue->postScrollEvent(deltaX, deltaY);
}

// MARK: - Keys

- (void)keyDown:(NSEvent *)event {
    if (eventQueue == nil) { return; }
    Panda::Key key = cocoaKeyToPandaKey(m_helper, [event keyCode]);
    m_helper.keys[(int)key] = true;
    eventQueue->postKeyEvent(key, true);
    [self interpretKeyEvents:@[event]];
}

- (void)flagsChanged:(NSEvent *)event {
    Panda::Key key = cocoaKeyToPandaKey(m_helper, [event keyCode]);
    const NSUInteger keyFlag = pandaKeyToCocoaModifierFlag(key);
    const NSUInteger modifierFlags = [event modifierFlags] & NSEventModifierFlagDeviceIndependentFlagsMask;
    bool down = keyFlag & modifierFlags;
    m_helper.keys[(int)key] = down;
    eventQueue->postKeyEvent(key, down);
}

- (void)keyUp:(NSEvent *)event {
    if (eventQueue == nil) { return; }
    Panda::Key key = cocoaKeyToPandaKey(m_helper, [event keyCode]);
    m_helper.keys[(int)key] = false;
    eventQueue->postKeyEvent(key, false);
}

// MARK: Text input client

- (BOOL)hasMarkedText {
    return [markedText length] > 0;
}

- (NSRange)markedRange {
    if ([markedText length] > 0) {
        return NSMakeRange(0, [markedText length] - 1);
    } else {
        return kEmptyRange;
    }
}

- (NSRange)selectedRange {
    return kEmptyRange;
}

- (void)setMarkedText:(id)string
        selectedRange:(NSRange)selectedRange
     replacementRange:(NSRange)replacementRange {
    [markedText release];
    if ([string isKindOfClass:[NSAttributedString class]]) {
        markedText = [[NSMutableAttributedString alloc] initWithAttributedString:string];
    } else {
        markedText = [[NSMutableAttributedString alloc] initWithString:string];
    }
}

- (void)unmarkText {
    [[markedText mutableString] setString:@""];
}

- (NSArray*)validAttributesForMarkedText {
    return [NSArray array];
}

- (NSAttributedString*)attributedSubstringForProposedRange:(NSRange)range
                                               actualRange:(NSRangePointer)actualRange {
    return nil;
}

- (NSUInteger)characterIndexForPoint:(NSPoint)point {
    return 0;
}

- (NSRect)firstRectForCharacterRange:(NSRange)range
                         actualRange:(NSRangePointer)actualRange {
    const NSRect frame = [self frame];
    return NSMakeRect(frame.origin.x, frame.origin.y, 0.0, 0.0);
}

- (void)insertText:(id)string replacementRange:(NSRange)replacementRange {
    if (eventQueue == nil) { return; }
    NSString* characters;
    if ([string isKindOfClass:[NSAttributedString class]]) {
        characters = [string string];
    } else {
        characters = (NSString*) string;
    }

    NSRange range = NSMakeRange(0, [characters length]);
    while (range.length) {
        uint32_t codepoint = 0;
        if ([characters getBytes:&codepoint
                       maxLength:sizeof(codepoint)
                      usedLength:NULL
                        encoding:NSUTF32StringEncoding
                         options:0
                           range:range
                  remainingRange:&range]) {
            if (codepoint >= 0xf700 && codepoint <= 0xf7ff) {
                continue;
            }
            eventQueue->postCharEvent(codepoint);
        }
    }
}

- (void)doCommandBySelector:(SEL)selector {}

@end

