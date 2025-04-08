//
//  WonderController.swift
//  Wonderland
//
//  Created by Admin on 02.08.2021.
//

#import "WonderController.h"
#import "WonderView.h"

@implementation WonderController {
    WonderView* customView;
}

- (void)loadView {
    customView = [[WonderView alloc] init];
    self.view = customView;
}

@end
