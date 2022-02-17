//
//  WonderController.swift
//  Wonderland
//
//  Created by Admin on 02.08.2021.
//

#import "WonderController.hpp"
#import "View/WonderView.hpp"

@implementation WonderController {
    WonderView* customView;
}

- (void)loadView {
    customView = [[WonderView alloc] init];
    self.view = customView;
}

@end
