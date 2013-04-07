// DetailViewController.h
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

#import <UIKit/UIKit.h>

@interface DetailViewController : UIViewController <UISplitViewControllerDelegate>

@property (nonatomic, retain) UIViewController *content;

- (void)clearCachedData;
- (void)editDetailPage:(id)sender;
- (void)saveDetailPage:(id)sender;

@end
