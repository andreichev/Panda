//
// Created by Admin on 17.02.2022.
//

#import "UIKitWindow.h"
#import "../AppDelegate.h"
#import "GraphicsContext/GLES_Context.h"

#import <UIKit/UIKit.h>

UIKitWindow::UIKitWindow(Fern::DrawingContextType contextType) {
    CGSize size = UIScreen.mainScreen.bounds.size;
    m_size = Fern::Size(size.width, size.height);
    CGFloat dpi = UIScreen.mainScreen.nativeScale;
    m_dpi = Fern::Size(dpi, dpi);
    dispatch_sync(dispatch_get_main_queue(), ^{
        auto delegate = (WonderAppDelegate*)[UIApplication sharedApplication].delegate;
        UIViewController *rootViewController = delegate.window.rootViewController;
        m_context = new GLES_Context((CAEAGLLayer*)[rootViewController view].layer);
    });
}
