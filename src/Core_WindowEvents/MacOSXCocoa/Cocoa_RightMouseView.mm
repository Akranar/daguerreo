
#include <iostream>
#import "Cocoa_RightMouseView.h"
#include "Cocoa_WindowData.h"
#include "../WindowEvents.h"


@implementation RightMouseView : NSView

-(void) rightMouseDown:(NSEvent *)theEvent
{
	[[self window] rightMouseDown:theEvent];
}

-(void) mouseEntered:(NSEvent *)theEvent
{
	//std::cout << "Entered" << std::endl;
}

-(void) mouseExited:(NSEvent *)theEvent
{
	//std::cout << "Exited" << std::endl;
	WindowData_Cocoa * window = (WindowData_Cocoa *) [self window];
	NSPoint mouse_position = [window mouseLocationOutsideOfEventStream];
	NSRect content_rect = [[window contentView] frame];
	
	int mouse_x = mouse_position.x;
	int mouse_y = mouse_position.y;
	mouse_y = content_rect.size.height - mouse_y;
	
	
	int new_mouse_x = mouse_x;
	int new_mouse_y = mouse_y;
	if (mouse_x < 0) new_mouse_x = 0;
	if (mouse_x > content_rect.size.width-2) new_mouse_x = content_rect.size.width-2;
	if (mouse_y < 0) new_mouse_y = 0;
	if (mouse_y > content_rect.size.height-2) new_mouse_y = content_rect.size.height-2;
	
	[window WindowEvents]->SetMousePosition(new_mouse_x, new_mouse_y);
}

-(NSTrackingRectTag) addTrackingRect:(NSRect)aRect owner:(id)anObject userData:(void *)data assumeInside:(BOOL)flag
{
	if (!tracking_rect) 
	{
		tracking_rect = [super addTrackingRect:aRect owner:anObject userData:data assumeInside:flag];
	}
	
	return tracking_rect;
}

-(void) removeTrackingRect
{
	if (tracking_rect)
	{
		[super removeTrackingRect:tracking_rect];
		tracking_rect = 0;
	}
}

-(void) dealloc
{
	[super dealloc];
}

@end
