#import <Cocoa/Cocoa.h>

#include "Cocoa_WindowData.h"
#include "Cocoa_RightMouseView.h"
#include "Cocoa_WindowDelegate.h"
#include "../WindowEvents.h"

#include <set>

#define PushInputEvent(input_event) WindowEvents::GetActive()->GetInputManager().PushEvent(input_event);

WindowData::~WindowData() 
{ 
	if (cocoa) 
	{
		[cocoa close]; //also releases since IsReleasedWhenClosed is true.
	}
	if (rm_view)
	{
		[rm_view release];
	}
	if (delegate)
	{
		[delegate release];
	}
};

int GetBitsPerPixel(CGDisplayModeRef & mode)
{
	size_t depth = 0;
	
	CFStringRef pixEnc = CGDisplayModeCopyPixelEncoding(mode);
	if(CFStringCompare(pixEnc, CFSTR(IO32BitDirectPixels), kCFCompareCaseInsensitive) == kCFCompareEqualTo)
		depth = 32;
	else if(CFStringCompare(pixEnc, CFSTR(IO16BitDirectPixels), kCFCompareCaseInsensitive) == kCFCompareEqualTo)
		depth = 16;
	else if(CFStringCompare(pixEnc, CFSTR(IO8BitIndexedPixels), kCFCompareCaseInsensitive) == kCFCompareEqualTo)
		depth = 8;
	
	return depth;
}

@implementation WindowData_Cocoa : NSWindow


static WindowEvents * active_window = 0;
static int instance_count = 0;
static std::set<WindowData_Cocoa *> window_data_set;

+ (WindowEvents *) GetActiveWindow
{
	return active_window;
}
+ (void) SetActiveWindow: (WindowEvents *) window_events
{
	active_window = window_events;
}
+ (WindowEvents *) FindWindow: (WindowData_Cocoa *) window_data
{
	std::set<WindowData_Cocoa *>::iterator it = window_data_set.find(window_data);
	if (it != window_data_set.end())
	{
		return [*it WindowEvents];
	}
	else
	{
		return 0;
	}
}

- (bool) GetMouseNeedsUpdate
{
	return need_mouse_update;
}

- (void) SetMouseNeedsUpdate: (bool) update
{
	need_mouse_update = update;
}

- (void) LaunchApplication
{
	[NSApp finishLaunching];
}

- (WindowEvents *) WindowEvents
{
	return window_events;
}

//Overrides:

- (id)InitWithContentRect:(NSRect) content_rect StyleMask:(NSUInteger) window_style WindowEvents: (WindowEvents *) in_window_events
{
	instance_count += 1;
	window_events = in_window_events;
	WindowData_Cocoa * rtn = [super initWithContentRect:content_rect styleMask:window_style backing:NSBackingStoreBuffered defer:NO];
	window_data_set.insert(self);
	return rtn;
}
-(void) dealloc
{
	[super dealloc];
}

-(void) close
{
	window_data_set.erase(self);
	[super close];
}

- (BOOL) isReleasedWhenClosed
{
	return YES;
}

- (BOOL)canBecomeKeyWindow
{
    return YES;
}

-(void) becomeKeyWindow
{
    //WindowEvents * matching_window = WindowData::FindWindow(self);
	WindowEvents * matching_window = [self WindowEvents];
    if (matching_window)
    {
        matching_window->MakeActive();
    }
	
	InputEvent input_event;
	input_event.SetType(InputEvent::WINDOW_ACTIVATION);
	input_event.data.window_activation.SetActive(true);
	PushInputEvent(input_event);
	
	need_mouse_update = true;

	[super becomeKeyWindow];
}

-(void) resignKeyWindow
{
	InputEvent input_event;
	input_event.SetType(InputEvent::WINDOW_ACTIVATION);
	input_event.data.window_activation.SetActive(false);
	PushInputEvent(input_event);
	
	[super resignKeyWindow];
}

-(void) mouseDown: (NSEvent *) theEvent
{
	InputEvent input_event;

	if (need_mouse_update)
	{
		NSPoint mouse_location = [theEvent locationInWindow];
		
		int current_x = (int) mouse_location.x;
		int current_y = ([[self contentView] frame].size.height - (int) mouse_location.y);
		
		input_event.SetType(InputEvent::MOUSE_MOTION);
		input_event.data.mouse_motion.SetX(current_x);
		input_event.data.mouse_motion.SetY(current_y);
		input_event.data.mouse_motion.SetRelX(0);
		input_event.data.mouse_motion.SetRelY(0);
		PushInputEvent(input_event);
		
		need_mouse_update = false;
	}
	
	input_event.SetType(InputEvent::MOUSE_BUTTON);
	input_event.data.mouse_button.SetButton(MouseButtonEvent::BUTTON_LEFT);
	input_event.data.mouse_button.SetState(MouseButtonEvent::PRESSED);
	PushInputEvent(input_event);
}

-(void) mouseUp: (NSEvent *) theEvent
{
    InputEvent input_event;
	
	if (need_mouse_update)
	{
		NSPoint mouse_location = [theEvent locationInWindow];
		
		int current_x = (int) mouse_location.x;
		int current_y = ([[self contentView] frame].size.height - (int) mouse_location.y);
		
		input_event.SetType(InputEvent::MOUSE_MOTION);
		input_event.data.mouse_motion.SetX(current_x);
		input_event.data.mouse_motion.SetY(current_y);
		input_event.data.mouse_motion.SetRelX(0);
		input_event.data.mouse_motion.SetRelY(0);
		PushInputEvent(input_event);
		
		need_mouse_update = false;
	}
	
	input_event.SetType(InputEvent::MOUSE_BUTTON);
	input_event.data.mouse_button.SetButton(MouseButtonEvent::BUTTON_LEFT);
	input_event.data.mouse_button.SetState(MouseButtonEvent::RELEASED);
	PushInputEvent(input_event);
}

-(void) mouseMoved: (NSEvent *) theEvent
{
    NSPoint mouse_location = [theEvent locationInWindow];
	
	int current_x = (int) mouse_location.x;
	int current_y = ([[self contentView] frame].size.height - (int) mouse_location.y);
	InputEvent input_event;
	input_event.SetType(InputEvent::MOUSE_MOTION);
	input_event.data.mouse_motion.SetX(current_x);
	input_event.data.mouse_motion.SetY(current_y);
	
	input_event.data.mouse_motion.SetRelX([theEvent deltaX]);
	input_event.data.mouse_motion.SetRelY([theEvent deltaY]);
	
	PushInputEvent(input_event);
}

-(void) mouseDragged: (NSEvent *) theEvent
{
	NSPoint mouse_location = [theEvent locationInWindow];
	
	int current_x = (int) mouse_location.x;
	int current_y = ([[self contentView] frame].size.height - (int) mouse_location.y);
	InputEvent input_event;
	input_event.SetType(InputEvent::MOUSE_MOTION);
	input_event.data.mouse_motion.SetX(current_x);
	input_event.data.mouse_motion.SetY(current_y);
	
	input_event.data.mouse_motion.SetRelX([theEvent deltaX]);
	input_event.data.mouse_motion.SetRelY([theEvent deltaY]);
	
	PushInputEvent(input_event);
}

-(void) scrollWheel: (NSEvent *) theEvent
{
	InputEvent input_event;
	if (need_mouse_update)
	{
		NSPoint mouse_location = [theEvent locationInWindow];
		
		int current_x = (int) mouse_location.x;
		int current_y = ([[self contentView] frame].size.height - (int) mouse_location.y);
		
		input_event.SetType(InputEvent::MOUSE_MOTION);
		input_event.data.mouse_motion.SetX(current_x);
		input_event.data.mouse_motion.SetY(current_y);
		input_event.data.mouse_motion.SetRelX(0);
		input_event.data.mouse_motion.SetRelY(0);
		PushInputEvent(input_event);

		need_mouse_update = false;
	}
	
	int delta_y = [theEvent deltaY];
	
	if (delta_y < 0)
	{
		input_event.SetType(InputEvent::MOUSE_WHEEL);
		input_event.data.mouse_wheel.SetDirection(MouseWheelEvent::DOWN);
		PushInputEvent(input_event);
	}
	else if (delta_y > 0)
	{
		input_event.SetType(InputEvent::MOUSE_WHEEL);
		input_event.data.mouse_wheel.SetDirection(MouseWheelEvent::UP);
		PushInputEvent(input_event);
	}

}

-(void) rightMouseDown: (NSEvent *) theEvent
{
	InputEvent input_event;
	if (need_mouse_update)
	{
		NSPoint mouse_location = [theEvent locationInWindow];
		
		int current_x = (int) mouse_location.x;
		int current_y = ([[self contentView] frame].size.height - (int) mouse_location.y);
		
		input_event.SetType(InputEvent::MOUSE_MOTION);
		input_event.data.mouse_motion.SetX(current_x);
		input_event.data.mouse_motion.SetY(current_y);
		input_event.data.mouse_motion.SetRelX(0);
		input_event.data.mouse_motion.SetRelY(0);
		PushInputEvent(input_event);

		need_mouse_update = false;
	}
	
	input_event.SetType(InputEvent::MOUSE_BUTTON);
	input_event.data.mouse_button.SetButton(MouseButtonEvent::BUTTON_RIGHT);
	input_event.data.mouse_button.SetState(MouseButtonEvent::PRESSED);
	PushInputEvent(input_event);
}

-(void) rightMouseUp: (NSEvent *) theEvent
{
	InputEvent input_event;
	if (need_mouse_update)
	{
		NSPoint mouse_location = [theEvent locationInWindow];
		
		int current_x = (int) mouse_location.x;
		int current_y = ([[self contentView] frame].size.height - (int) mouse_location.y);
		
		input_event.SetType(InputEvent::MOUSE_MOTION);
		input_event.data.mouse_motion.SetX(current_x);
		input_event.data.mouse_motion.SetY(current_y);
		input_event.data.mouse_motion.SetRelX(0);
		input_event.data.mouse_motion.SetRelY(0);
		
		PushInputEvent(input_event);
		need_mouse_update = false;
	}
	
	input_event.SetType(InputEvent::MOUSE_BUTTON);
	input_event.data.mouse_button.SetButton(MouseButtonEvent::BUTTON_RIGHT);
	input_event.data.mouse_button.SetState(MouseButtonEvent::RELEASED);
	PushInputEvent(input_event);
}

-(void) rightMouseDragged: (NSEvent *) theEvent
{
	NSPoint mouse_location = [theEvent locationInWindow];
	
	int current_x = (int) mouse_location.x;
	int current_y = ([[self contentView] frame].size.height - (int) mouse_location.y);
	InputEvent input_event;
	input_event.SetType(InputEvent::MOUSE_MOTION);
	input_event.data.mouse_motion.SetX(current_x);
	input_event.data.mouse_motion.SetY(current_y);
	input_event.data.mouse_motion.SetRelX([theEvent deltaX]);
	input_event.data.mouse_motion.SetRelY([theEvent deltaY]);
	
	PushInputEvent(input_event);
}

-(void) otherMouseDown: (NSEvent *) theEvent
{
	InputEvent input_event;
	if (need_mouse_update)
	{
		NSPoint mouse_location = [theEvent locationInWindow];
		
		int current_x = (int) mouse_location.x;
		int current_y = ([[self contentView] frame].size.height - (int) mouse_location.y);
		
		input_event.SetType(InputEvent::MOUSE_MOTION);
		input_event.data.mouse_motion.SetX(current_x);
		input_event.data.mouse_motion.SetY(current_y);
		input_event.data.mouse_motion.SetRelX(0);
		input_event.data.mouse_motion.SetRelY(0);
		PushInputEvent(input_event);
		
		need_mouse_update = false;
	}
	
	input_event.SetType(InputEvent::MOUSE_BUTTON);
	input_event.data.mouse_button.SetButton(MouseButtonEvent::BUTTON_MIDDLE);
	input_event.data.mouse_button.SetState(MouseButtonEvent::PRESSED);
	PushInputEvent(input_event);
}

-(void) otherMouseUp: (NSEvent *) theEvent
{
	InputEvent input_event;
	if (need_mouse_update)
	{
		NSPoint mouse_location = [theEvent locationInWindow];
		
		int current_x = (int) mouse_location.x;
		int current_y = ([[self contentView] frame].size.height - (int) mouse_location.y);
		
		input_event.SetType(InputEvent::MOUSE_MOTION);
		input_event.data.mouse_motion.SetX(current_x);
		input_event.data.mouse_motion.SetY(current_y);
		input_event.data.mouse_motion.SetRelX(0);
		input_event.data.mouse_motion.SetRelY(0);
		PushInputEvent(input_event);

		need_mouse_update = false;
	}
	
	input_event.SetType(InputEvent::MOUSE_BUTTON);
	input_event.data.mouse_button.SetButton(MouseButtonEvent::BUTTON_MIDDLE);
	input_event.data.mouse_button.SetState(MouseButtonEvent::RELEASED);
	PushInputEvent(input_event);
}

-(void) otherMouseDragged: (NSEvent *) theEvent
{
	NSPoint mouse_location = [theEvent locationInWindow];
	
	int current_x = (int) mouse_location.x;
	int current_y = ([[self contentView] frame].size.height - (int) mouse_location.y);
	InputEvent input_event;
	input_event.SetType(InputEvent::MOUSE_MOTION);
	input_event.data.mouse_motion.SetX(current_x);
	input_event.data.mouse_motion.SetY(current_y);
	input_event.data.mouse_motion.SetRelX([theEvent deltaX]);
	input_event.data.mouse_motion.SetRelY([theEvent deltaY]);
	
	PushInputEvent(input_event);
}

- (void) sendEvent: (NSEvent*)theEvent
{
    [super sendEvent: theEvent];
}


@end
