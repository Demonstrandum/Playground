#import <QuartzCore/CAMetalLayer.h>
#import <Metal/Metal.h>
#import <MetalKit/MetalKit.h>
#import <Cocoa/Cocoa.h>


namespace native {
	void *metalLayer(void *cocoaWindow) {
		NSWindow *window = (NSWindow*)cocoaWindow;
		NSView *contentView = [window contentView];
		[contentView setWantsLayer:YES];
		CAMetalLayer *res = [CAMetalLayer layer];
		[contentView setLayer:res];
		return res;
	}
}
