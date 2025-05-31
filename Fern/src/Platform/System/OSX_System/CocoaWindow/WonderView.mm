//
//  WonderView.mm
//
//  Created by Andreichev Mikhail on 07.02.2025.
//

#import "WonderView.h"
#import "Fern/Events/EventQueue.hpp"

using namespace Fern;

namespace Fern {
EventQueue* getEventQueue();
bool isCursorLocked();
}

struct WonderHelper {
    Fern::Key keycodes[256];
    Fern::Key scancodes[256];
    bool keys[256];
};

static const NSRange kEmptyRange = { NSNotFound, 0 };

static void fillKeyTable(WonderHelper& helper) {
    memset(helper.keycodes, -1, sizeof(helper.keycodes));
    memset(helper.scancodes, -1, sizeof(helper.scancodes));

    helper.keycodes[0x1D] = Fern::Key::KEY_0;
    helper.keycodes[0x12] = Fern::Key::KEY_1;
    helper.keycodes[0x13] = Fern::Key::KEY_2;
    helper.keycodes[0x14] = Fern::Key::KEY_3;
    helper.keycodes[0x15] = Fern::Key::KEY_4;
    helper.keycodes[0x17] = Fern::Key::KEY_5;
    helper.keycodes[0x16] = Fern::Key::KEY_6;
    helper.keycodes[0x1A] = Fern::Key::KEY_7;
    helper.keycodes[0x1C] = Fern::Key::KEY_8;
    helper.keycodes[0x19] = Fern::Key::KEY_9;
    helper.keycodes[0x00] = Fern::Key::A;
    helper.keycodes[0x0B] = Fern::Key::B;
    helper.keycodes[0x08] = Fern::Key::C;
    helper.keycodes[0x02] = Fern::Key::D;
    helper.keycodes[0x0E] = Fern::Key::E;
    helper.keycodes[0x03] = Fern::Key::F;
    helper.keycodes[0x05] = Fern::Key::G;
    helper.keycodes[0x04] = Fern::Key::H;
    helper.keycodes[0x22] = Fern::Key::I;
    helper.keycodes[0x26] = Fern::Key::J;
    helper.keycodes[0x28] = Fern::Key::K;
    helper.keycodes[0x25] = Fern::Key::L;
    helper.keycodes[0x2E] = Fern::Key::M;
    helper.keycodes[0x2D] = Fern::Key::N;
    helper.keycodes[0x1F] = Fern::Key::O;
    helper.keycodes[0x23] = Fern::Key::P;
    helper.keycodes[0x0C] = Fern::Key::Q;
    helper.keycodes[0x0F] = Fern::Key::R;
    helper.keycodes[0x01] = Fern::Key::S;
    helper.keycodes[0x11] = Fern::Key::T;
    helper.keycodes[0x20] = Fern::Key::U;
    helper.keycodes[0x09] = Fern::Key::V;
    helper.keycodes[0x0D] = Fern::Key::W;
    helper.keycodes[0x07] = Fern::Key::X;
    helper.keycodes[0x10] = Fern::Key::Y;
    helper.keycodes[0x06] = Fern::Key::Z;

    helper.keycodes[0x27] = Fern::Key::APOSTROPHE;
    helper.keycodes[0x2A] = Fern::Key::BACKSLASH;
    helper.keycodes[0x2B] = Fern::Key::COMMA;
    helper.keycodes[0x18] = Fern::Key::EQUAL;
    helper.keycodes[0x32] = Fern::Key::GRAVE_ACCENT;
    helper.keycodes[0x21] = Fern::Key::LEFT_BRACKET;
    helper.keycodes[0x1B] = Fern::Key::MINUS;
    helper.keycodes[0x2F] = Fern::Key::PERIOD;
    helper.keycodes[0x1E] = Fern::Key::RIGHT_BRACKET;
    helper.keycodes[0x29] = Fern::Key::SEMICOLON;
    helper.keycodes[0x2C] = Fern::Key::SLASH;
    helper.keycodes[0x0A] = Fern::Key::WORLD_1;

    helper.keycodes[0x33] = Fern::Key::BACKSPACE;
    helper.keycodes[0x39] = Fern::Key::CAPS_LOCK;
    helper.keycodes[0x75] = Fern::Key::DELETE;
    helper.keycodes[0x7D] = Fern::Key::DOWN;
    helper.keycodes[0x77] = Fern::Key::END;
    helper.keycodes[0x24] = Fern::Key::ENTER;
    helper.keycodes[0x35] = Fern::Key::ESCAPE;
    helper.keycodes[0x7A] = Fern::Key::F1;
    helper.keycodes[0x78] = Fern::Key::F2;
    helper.keycodes[0x63] = Fern::Key::F3;
    helper.keycodes[0x76] = Fern::Key::F4;
    helper.keycodes[0x60] = Fern::Key::F5;
    helper.keycodes[0x61] = Fern::Key::F6;
    helper.keycodes[0x62] = Fern::Key::F7;
    helper.keycodes[0x64] = Fern::Key::F8;
    helper.keycodes[0x65] = Fern::Key::F9;
    helper.keycodes[0x6D] = Fern::Key::F10;
    helper.keycodes[0x67] = Fern::Key::F11;
    helper.keycodes[0x6F] = Fern::Key::F12;
    helper.keycodes[0x69] = Fern::Key::PRINT_SCREEN;
    helper.keycodes[0x6B] = Fern::Key::F14;
    helper.keycodes[0x71] = Fern::Key::F15;
    helper.keycodes[0x6A] = Fern::Key::F16;
    helper.keycodes[0x40] = Fern::Key::F17;
    helper.keycodes[0x4F] = Fern::Key::F18;
    helper.keycodes[0x50] = Fern::Key::F19;
    helper.keycodes[0x5A] = Fern::Key::F20;
    helper.keycodes[0x73] = Fern::Key::HOME;
    helper.keycodes[0x72] = Fern::Key::INSERT;
    helper.keycodes[0x7B] = Fern::Key::LEFT;
    helper.keycodes[0x3A] = Fern::Key::LEFT_ALT;
    helper.keycodes[0x3B] = Fern::Key::LEFT_CONTROL;
    helper.keycodes[0x38] = Fern::Key::LEFT_SHIFT;
    helper.keycodes[0x37] = Fern::Key::LEFT_SUPER;
    helper.keycodes[0x6E] = Fern::Key::MENU;
    helper.keycodes[0x47] = Fern::Key::NUM_LOCK;
    helper.keycodes[0x79] = Fern::Key::PAGE_DOWN;
    helper.keycodes[0x74] = Fern::Key::PAGE_UP;
    helper.keycodes[0x7C] = Fern::Key::RIGHT;
    helper.keycodes[0x3D] = Fern::Key::RIGHT_ALT;
    helper.keycodes[0x3E] = Fern::Key::RIGHT_CONTROL;
    helper.keycodes[0x3C] = Fern::Key::RIGHT_SHIFT;
    helper.keycodes[0x36] = Fern::Key::RIGHT_SUPER;
    helper.keycodes[0x31] = Fern::Key::SPACE;
    helper.keycodes[0x30] = Fern::Key::TAB;
    helper.keycodes[0x7E] = Fern::Key::UP;

    helper.keycodes[0x52] = Fern::Key::KP_0;
    helper.keycodes[0x53] = Fern::Key::KP_1;
    helper.keycodes[0x54] = Fern::Key::KP_2;
    helper.keycodes[0x55] = Fern::Key::KP_3;
    helper.keycodes[0x56] = Fern::Key::KP_4;
    helper.keycodes[0x57] = Fern::Key::KP_5;
    helper.keycodes[0x58] = Fern::Key::KP_6;
    helper.keycodes[0x59] = Fern::Key::KP_7;
    helper.keycodes[0x5B] = Fern::Key::KP_8;
    helper.keycodes[0x5C] = Fern::Key::KP_9;
    helper.keycodes[0x45] = Fern::Key::KP_ADD;
    helper.keycodes[0x41] = Fern::Key::KP_DECIMAL;
    helper.keycodes[0x4B] = Fern::Key::KP_DIVIDE;
    helper.keycodes[0x4C] = Fern::Key::KP_ENTER;
    helper.keycodes[0x51] = Fern::Key::KP_EQUAL;
    helper.keycodes[0x43] = Fern::Key::KP_MULTIPLY;
    helper.keycodes[0x4E] = Fern::Key::KP_SUBTRACT;

    for (int scancode = 0;  scancode < 256;  scancode++) {
        if ((int)helper.keycodes[scancode] >= 0)
            helper.scancodes[(int)helper.keycodes[scancode]] = (Fern::Key)scancode;
    }
}

static Fern::Key cocoaKeyToPandaKey(WonderHelper& helper, unsigned int key) {
    if (key >= sizeof(helper.keycodes) / sizeof(helper.keycodes[0])) {
        return Fern::Key::UNKNOWN;
    }
    return helper.keycodes[key];
}

static NSUInteger pandaKeyToCocoaModifierFlag(Fern::Key key) {
    switch (key) {
        case Fern::Key::LEFT_SHIFT:
        case Fern::Key::RIGHT_SHIFT:
            return NSEventModifierFlagShift;
        case Fern::Key::LEFT_CONTROL:
        case Fern::Key::RIGHT_CONTROL:
            return NSEventModifierFlagControl;
        case Fern::Key::LEFT_ALT:
        case Fern::Key::RIGHT_ALT:
            return NSEventModifierFlagOption;
        case Fern::Key::LEFT_SUPER:
        case Fern::Key::RIGHT_SUPER:
            return NSEventModifierFlagCommand;
        case Fern::Key::CAPS_LOCK:
            return NSEventModifierFlagCapsLock;
        default: return 0;
    }
}

@implementation WonderView {
    WonderHelper m_helper;
    BOOL m_multiTouchScroll;
}

- (instancetype)init {
    self = [super init];
    if (self != nil) {
        memset(m_helper.keys, 0, sizeof(m_helper.keys));
        markedText = [[NSMutableAttributedString alloc] init];
        fillKeyTable(m_helper);
        m_multiTouchScroll = NO;
        self.ignoreMouseEvents = 0;
    }
    NSLog(@"WonderView created");
    return self;
}

- (void)dealloc {
    [super dealloc];
    [markedText release];
    NSLog(@"WonderView deallocated");
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

// MARK: - Mouse

- (void)mouseMoved:(NSEvent *)event {
    // NOTE: The returned location uses base 0,1 not 0,0
    const NSPoint pos = [event locationInWindow];
    double xpos = pos.x;
    double ypos = self.frame.size.height - pos.y;
    double deltaX = [event deltaX];
    double deltaY = [event deltaY];
    // Если мы переместили курсор программно, нам не нужно иметь дельты
    if (self.ignoreMouseEvents) {
        deltaX = 0;
        deltaY = 0;
        self.ignoreMouseEvents--;
    }
    getEventQueue()->postMouseEvent(xpos, ypos, deltaX, deltaY);
}

- (void)mouseDragged:(NSEvent *)event {
    [self mouseMoved:event];
}

- (void)rightMouseDragged:(NSEvent *)event {
    [self mouseMoved:event];
}

- (void)mouseDown:(NSEvent *)event {
    getEventQueue()->postMouseButtonEvent(MouseButton::LEFT, true);
}

- (void)mouseUp:(NSEvent *)event {
    getEventQueue()->postMouseButtonEvent(MouseButton::LEFT, false);
}

- (void)rightMouseDown:(NSEvent *)event {
    getEventQueue()->postMouseButtonEvent(MouseButton::RIGHT, true);
}

- (void)rightMouseUp:(NSEvent *)event {
    getEventQueue()->postMouseButtonEvent(MouseButton::RIGHT, false);
}

- (void)otherMouseDown:(NSEvent *)event {
    getEventQueue()->postMouseButtonEvent(MouseButton::MIDDLE, true);
}

- (void)otherMouseUp:(NSEvent *)event {
    getEventQueue()->postMouseButtonEvent(MouseButton::RIGHT, false);
}

- (void)scrollWheel:(NSEvent *)event {
    double deltaX = event.scrollingDeltaX;
    double deltaY = event.scrollingDeltaY;
    if (event.phase == NSEventPhaseBegan) {
        m_multiTouchScroll = true;
    } else if (event.phase == NSEventPhaseEnded) {
        m_multiTouchScroll = false;
    }
    if ([event hasPreciseScrollingDeltas]) {
        deltaX *= 0.025;
        deltaY *= 0.025;
    }
    getEventQueue()->postScrollEvent(deltaX, deltaY, m_multiTouchScroll || event.momentumPhase != NSEventPhaseNone);
}

// MARK: - Keys

- (void)keyDown:(NSEvent *)event {
    Fern::Key key = cocoaKeyToPandaKey(m_helper, [event keyCode]);
    m_helper.keys[(int)key] = true;
    getEventQueue()->postKeyEvent(key, true);
    [self interpretKeyEvents:@[event]];
}

- (void)flagsChanged:(NSEvent *)event {
    Fern::Key key = cocoaKeyToPandaKey(m_helper, [event keyCode]);
    const NSUInteger keyFlag = pandaKeyToCocoaModifierFlag(key);
    const NSUInteger modifierFlags = [event modifierFlags] & NSEventModifierFlagDeviceIndependentFlagsMask;
    bool down = keyFlag & modifierFlags;
    m_helper.keys[(int)key] = down;
    getEventQueue()->postKeyEvent(key, down);
}

- (void)keyUp:(NSEvent *)event {
    Fern::Key key = cocoaKeyToPandaKey(m_helper, [event keyCode]);
    m_helper.keys[(int)key] = false;
    getEventQueue()->postKeyEvent(key, false);
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
            getEventQueue()->postCharEvent(codepoint);
        }
    }
}

- (void)doCommandBySelector:(SEL)selector {}

@end

