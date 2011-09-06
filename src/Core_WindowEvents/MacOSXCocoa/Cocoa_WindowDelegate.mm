#include "Cocoa_WindowDelegate.h"
#include "../WindowEvents.h"
#include "Cocoa_WindowData.h"
#include "../RenderContext.h"
#include "RenderContextPimpl_MACOSX.h"

#include <iostream>



@implementation WindowDelegate : NSObject

- (bool) windowShouldClose: (id)sender
{
	//Return false so window doesn't close.  We use our own close method.
    NSWindow * target = sender;
	WindowEvents * targeted_window = [WindowData_Cocoa FindWindow: (WindowData_Cocoa *) target];
    if (targeted_window)
    {
        targeted_window->Close();
    }
    return false;
}

- (NSSize) windowWillResize: (NSWindow *) sender toSize: (NSSize) frameSize
{
	InputEvent input_event;
	input_event.SetType(InputEvent::RESIZE);
	input_event.data.resize.SetWidth(frameSize.width);
	input_event.data.resize.SetHeight(frameSize.height);
	WindowEvents::GetActive()->GetInputManager().PushEvent(input_event);
	return frameSize;
}

- (void)windowDidResize:(NSNotification *)notification
{
	[[((WindowData_Cocoa *) [notification object]) WindowEvents]->GetRenderContext()->GetPimpl()->GetContextObject() update];
}

-(void) dealloc
{
	[super dealloc];
}
@end
