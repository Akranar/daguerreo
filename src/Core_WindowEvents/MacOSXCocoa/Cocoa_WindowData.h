#ifndef _COCOA_WINDOW_DATA_H_
#define _COCOA_WINDOW_DATA_H_

#import <Cocoa/Cocoa.h>

class WindowEvents;
@class RightMouseView;
@class WindowDelegate;

@interface WindowData_Cocoa : NSWindow
{
	bool need_mouse_update;
    WindowEvents * window_events;
}
+ (WindowEvents *) GetActiveWindow;
+ (void) SetActiveWindow: (WindowEvents *) window_events;
+ (WindowEvents *) FindWindow: (WindowData_Cocoa *) window_data;


- (id)InitWithContentRect:(NSRect)content_rect StyleMask:(NSUInteger) window_style WindowEvents: (WindowEvents *) in_window_events;
- (void) LaunchApplication;
- (void) CacheDisplayModesWithWidth: (int) fullscreen_width AndHeight: (int) fullscreen_height;
- (WindowEvents *) WindowEvents;
- (bool) GetMouseNeedsUpdate;
- (void) SetMouseNeedsUpdate: (bool) update;



@end

class WindowData
{
public:
	WindowData_Cocoa * cocoa;
	RightMouseView * rm_view;
	WindowDelegate * delegate;
    WindowData() 
	: 
	cocoa(0),
	rm_view(0),
	delegate(0)
	{
	};
    ~WindowData();
};



#endif
