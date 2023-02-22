#import <QuartzCore/CAMetalLayer.h>
#import <Metal/Metal.h>
#import <MetalKit/MetalKit.h>
#import <Cocoa/Cocoa.h>

#import <objc/runtime.h>

@interface FakeView : NSView
@end
@implementation FakeView
- (BOOL)fakeMouseDownCanMoveWindow { return YES; }
@end

namespace native {
	void *metalLayer(void *cocoaWindow) {
		/* mess with window */
		NSWindow *window = (NSWindow*)cocoaWindow;
		window.styleMask |= NSWindowStyleMaskUnifiedTitleAndToolbar
			| NSWindowStyleMaskFullSizeContentView;
		window.titlebarAppearsTransparent = NO;
		//window.titleVisibility = NSWindowTitleHidden;
		window.backgroundColor = [NSColor blackColor];
		window.toolbar.visible = YES;
		/* toolbar */
		window.toolbar = [NSToolbar alloc];
		/* view */
		NSView *contentView = [window contentView];
		/* swizzle move window method */
		static bool swizzled = false;
		if (!swizzled) {
			swizzled = true;
			Method originalMethod = class_getInstanceMethod([contentView class], @selector(mouseDownCanMoveWindow));
			Method categoryMethod = class_getInstanceMethod(FakeView.class, @selector(fakeMouseDownCanMoveWindow));
			method_exchangeImplementations(originalMethod, categoryMethod);
		}
		/* return metal layer */
		[contentView setWantsLayer:YES];
		CAMetalLayer *res = [CAMetalLayer layer];
		[contentView setLayer:res];
		return res;
	}
}
