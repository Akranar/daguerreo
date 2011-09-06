#import "../WindowEvents.h"
#import <Cocoa/Cocoa.h>
#import "Cocoa_WindowDelegate.h"
#import "Cocoa_WindowData.h"
#import "Cocoa_RightMouseView.h"
#include "KeyMapping_MACOSX.h"

#include "../RenderContext.h"
#include "RenderContextPimpl_MACOSX.h"

#include <iostream>

/*
    Relevant Links and Notes:
        -> Cocoa and GNUStep:
            -> http://forums.macrumors.com/showthread.php?t=791236
        -> NSApplication:

            Reference:
                -> http://developer.apple.com/library/mac/#documentation/Cocoa/Reference/ApplicationKit/Classes/NSApplication_Class/Reference/Reference.html
        
                finishLaunching
                    -> Activates the receiver, opens any files specified by the NSOpen user default, and unhighlights the application’s icon.
                    -> Called by Run method before it starts event loop.

            -> http://www.gnustep.it/nicola/Tutorials/FirstGUIApplication/node2.html
            -> http://cocoadev.com/index.pl?NSApplication

        -> NSOpenGLContext:
            -> http://developer.apple.com/library/mac/#documentation/Cocoa/Reference/ApplicationKit/Classes/NSOpenGLContext_Class/Reference/Reference.html

        -> Creating a Custom View:
            -> http://developer.apple.com/library/mac/#documentation/Cocoa/Conceptual/CocoaViewsGuide/SubclassingNSView/SubclassingNSView.html#//apple_ref/doc/uid/TP40002978-CH7

        -> Drawing OpenGL Content to a Custom View:
            -> http://developer.apple.com/library/mac/#documentation/GraphicsImaging/Conceptual/OpenGL-MacProgGuide/opengl_drawing/opengl_drawing.html#//apple_ref/doc/uid/TP40001987-CH404-SW3

        -> Event Handling
            -> http://developer.apple.com/library/mac/#documentation/Cocoa/Conceptual/EventOverview/Introduction/Introduction.html

        -> Launching Application
            -> Necessary for the Menu to function.

        -> Fullscreen
            -> http://cocoadevcentral.com/articles/000028.php
            -> http://www.cocoabuilder.com/archive/cocoa/128044-cocoa-full-screen-mode-and-windows.html

            -> CGDisplaySwitchToMode Deprecated.
                -> Instead use:
                    -> CGDisplaySetDisplayMode:
                        -> http://developer.apple.com/library/mac/#documentation/GraphicsImaging/Reference/Quartz_Services_Ref/Reference/reference.html#//apple_ref/c/func/CGDisplaySetDisplayMode

            -> Enumerating Display Modes:
                -> http://lukassen.wordpress.com/2010/01/18/taming-snow-leopard-cgdisplaybestmodeforparameters-deprecation/
            -> Bits Per Pixel:
                -> http://lukassen.wordpress.com/2010/01/18/taming-snow-leopard-cgdisplaybitsperpixel-deprication/
 
		-> Key Handling:
			-> http://www.cocoadev.com/index.pl?GameKeyBoardHandling
			-> http://www.cocoadev.com/index.pl?GameKeyboardHandlingAlmost


*/


int WindowEvents::window_count = 0;
int windowevents_instance_count = 0;
NSAutoreleasePool * master_pool = 0;
unsigned int frame = 0;
bool application_launched = false;

//Constructors
WindowEvents::WindowEvents() 
: 
width(800), 
height(600), 
auto_repeat(false),
initialized(false),
fullscreen(false),
render_context(0),
maximizable(true),
minimizable(true),
resizable(true)

{
	data = 0;
	windowevents_instance_count += 1;
	MakeActive();
};

WindowEvents::WindowEvents(int width, int height) 
: 
width(width), 
height(height), 
auto_repeat(false),
initialized(false),
fullscreen(false),
render_context(0),
maximizable(true),
minimizable(true),
resizable(true)
{
	data = 0;
	windowevents_instance_count += 1;
	MakeActive();
}

//Destructor

WindowEvents::~WindowEvents()
{
	if (initialized) Close();
	
	windowevents_instance_count -= 1;
	
	if (windowevents_instance_count == 0 && master_pool)
	{
		[master_pool drain];
		master_pool = 0;
	}
	
};
 

//Static Methods
WindowEvents * WindowEvents::GetActive()
{
	return [WindowData_Cocoa GetActiveWindow];
}


int WindowEvents::GetWindowCount()
{
	return window_count;
}

void WindowEvents::SetMousePosition(int x, int y)
{
	int screen_x, screen_y;
	ClientToScreenCoordinates(x, y, screen_x, screen_y);
	CGPoint screen;
	screen.x = screen_x;
	screen.y = screen_y;
    CGDisplayMoveCursorToPoint (kCGDirectMainDisplay, screen);

}

void WindowEvents::SetShowMouse(bool show)
{
	if (show)
	{
		CGDisplayShowCursor(kCGDirectMainDisplay);
	}
	else
	{
		CGDisplayHideCursor(kCGDirectMainDisplay);
	}
}

#define PushInputEvent(input_event) GetActive()->GetInputManager().PushEvent(input_event);



//Static Message Processing
int WindowEvents::ProcessEvents()
{
	if (!window_count) return 0;
	//NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
	
    NSEvent *event;
	NSDate *distantPast = [NSDate distantPast];
    event = [NSApp nextEventMatchingMask: NSAnyEventMask untilDate: distantPast inMode: NSDefaultRunLoopMode dequeue: YES];
    NSEventType eventType = [event type];

    while (event)
    {
        if (eventType == NSKeyDown)
        {
            if(![event isARepeat])
            {
                KeyID::Key key_id = KeyMap[[event keyCode]];
                InputEvent input_event;
				input_event.SetType(InputEvent::KEYBOARD);
				input_event.data.key.SetType(KeyEvent::KEY_DOWN);
				input_event.data.key.SetKey(key_id);
				PushInputEvent(input_event);
            } 
            else if (GetActive()->AutoRepeat())
            {
                KeyID::Key key_id = KeyMap[[event keyCode]];
                InputEvent input_event;
				input_event.SetType(InputEvent::KEYBOARD);
				input_event.data.key.SetType(KeyEvent::KEY_REPEAT);
				input_event.data.key.SetKey(key_id);
				PushInputEvent(input_event);
            }

        }
        else if (eventType == NSKeyUp)
        {
            //printf("Key Up: %d\n", [event keyCode]);
            
            KeyID::Key key_id = KeyMap[[event keyCode]];
            InputEvent input_event;
			input_event.SetType(InputEvent::KEYBOARD);
			input_event.data.key.SetType(KeyEvent::KEY_UP);
			input_event.data.key.SetKey(key_id);
			PushInputEvent(input_event);
        }
        else if (eventType == NSFlagsChanged)
        {
            NSInteger flags = [event modifierFlags];
            //const KeyState & key_state = GetActive()->GetInputManager().GetKeyState();
            
            KeyID::Key key_id = KeyMap[[event keyCode]];
            InputEvent input_event;
			input_event.SetType(InputEvent::KEYBOARD);
			input_event.data.key.SetKey(key_id);
			
			//Uses device_dependent codes for modifier flags.
			switch (key_id)
			{
				case KeyID::KEY_CAPSLOCK:
					input_event.data.key.SetType(KeyEvent::KEY_DOWN);
					PushInputEvent(input_event);
					break;
				case KeyID::KEY_LEFT_SHIFT:
					if (flags & 2) 
					{
						input_event.data.key.SetType(KeyEvent::KEY_DOWN);
						PushInputEvent(input_event);
					}
					else
					{
						input_event.data.key.SetType(KeyEvent::KEY_UP);
						PushInputEvent(input_event);
					}
					break;
				case KeyID::KEY_RIGHT_SHIFT:
					if (flags & 4) 
					{
						input_event.data.key.SetType(KeyEvent::KEY_DOWN);
						PushInputEvent(input_event);
					}
					else
					{
						input_event.data.key.SetType(KeyEvent::KEY_UP);
						PushInputEvent(input_event);
					}
					break;
				case KeyID::KEY_LEFT_CTRL:
					if (flags & 1) 
					{
						input_event.data.key.SetType(KeyEvent::KEY_DOWN);
						PushInputEvent(input_event);
					}
					else
					{
						input_event.data.key.SetType(KeyEvent::KEY_UP);
						PushInputEvent(input_event);
					}
					break;
				case KeyID::KEY_RIGHT_CTRL:
					//Apparently doesn't exist
					break;
				case KeyID::KEY_LEFT_ALT:
					if (flags & 32) 
					{
						input_event.data.key.SetType(KeyEvent::KEY_DOWN);
						PushInputEvent(input_event);
					}
					else
					{
						input_event.data.key.SetType(KeyEvent::KEY_UP);
						PushInputEvent(input_event);
					}
					break;
				case KeyID::KEY_RIGHT_ALT:
					if (flags & 64) 
					{
						input_event.data.key.SetType(KeyEvent::KEY_DOWN);
						PushInputEvent(input_event);
					}
					else
					{
						input_event.data.key.SetType(KeyEvent::KEY_UP);
						PushInputEvent(input_event);
					}
					break;
				case KeyID::KEY_LEFT_SUPER:
					if (flags & 8) 
					{
						input_event.data.key.SetType(KeyEvent::KEY_DOWN);
						PushInputEvent(input_event);
					}
					else
					{
						input_event.data.key.SetType(KeyEvent::KEY_UP);
						PushInputEvent(input_event);
					}
					break;
				case KeyID::KEY_RIGHT_SUPER:
					if (flags & 16) 
					{
						input_event.data.key.SetType(KeyEvent::KEY_DOWN);
						PushInputEvent(input_event);
					}
					else
					{
						input_event.data.key.SetType(KeyEvent::KEY_UP);
						PushInputEvent(input_event);
					}
					break;
				default:
					{
					}
					break;
			}
        }
        else 
        {
            [NSApp sendEvent:event];
        } 

		//[NSApp updateWindows];
        event = [NSApp nextEventMatchingMask: NSAnyEventMask untilDate: distantPast inMode: NSDefaultRunLoopMode dequeue: YES];
        eventType = [event type];
    }
	
	//Release pool ever 100 frames.
	frame += 1;
	if (frame > 100 && master_pool)
	{
		[master_pool drain];
		master_pool = [[NSAutoreleasePool alloc] init];
		frame = 0;
	}
    return GetWindowCount();
}



//Active Draw / Message Window
void WindowEvents::MakeActive()
{
	[WindowData_Cocoa SetActiveWindow:this];
}
void WindowEvents::MakeCurrentDraw()
{
	NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
	[render_context->GetPimpl()->GetContextObject() setView:[data->cocoa contentView]];
	[pool drain];
}


//Full Screen and Size Methods

bool WindowEvents::EnterFullscreen()
{   
	
    if (!initialized) return false;    

	//I do this instead of NSView's enterFullScreenMode in order to keep the event structure.
	SetShowTitleBar(false);
	int screen_width = CGDisplayBounds(kCGDirectMainDisplay).size.width;
	int screen_height = CGDisplayBounds(kCGDirectMainDisplay).size.height;
	SetSize(screen_width, screen_height);
	SetTopLeftPosition(0, 0);
	SetAlwaysOnTop(true);
	 
	//[[data->cocoa contentView] enterFullScreenMode:[NSScreen mainScreen] withOptions: nil];
	 
	return true;
}


void WindowEvents::ExitFullscreen()
{
	
    if (!initialized) return;

	SetTopLeftPosition(0, 0);
	SetSize(width, height);
	SetShowTitleBar(true);
	SetAlwaysOnTop(false);
	
	//[[data->cocoa contentView] exitFullScreenModeWithOptions:nil];
	 
}

bool WindowEvents::IsMaximized()
{
	return [data->cocoa isZoomed];
}

void WindowEvents::ToggleMaximize()
{
	[data->cocoa performZoom: NULL];
}
void WindowEvents::Minimize()
{
	[data->cocoa performMiniaturize: NULL];
}

void WindowEvents::SetSize(int w, int h)
{
	[data->cocoa setContentSize:NSMakeSize(w, h)];
	
	InputEvent input_event;
	input_event.SetType(InputEvent::RESIZE);
	input_event.data.resize.SetWidth(w);
	input_event.data.resize.SetHeight(h);
	GetInputManager().PushEvent(input_event);
}

//Window Visibility
void WindowEvents::Show()
{
    if (!initialized) return;
    visible = true;
	[data->cocoa makeKeyAndOrderFront: nil];
    
}
void WindowEvents::Hide()
{
    visible = false;
}

//Window Termination
void WindowEvents::Close()
{   
    if (!initialized) return;
    if (fullscreen) this->ExitFullscreen();
	
	//[this->data->cocoa close];
	if (data) delete data;
	data = 0;

    window_count -= 1;
	
    visible = false;
    initialized = false;
}

void WindowEvents::SetTitle(const char * text)
{
    if (!initialized) return; 
    NSString * title = [[[NSString alloc] autorelease] initWithCString:text encoding:NSMacOSRomanStringEncoding];
	[this->data->cocoa setTitle:title];
	[title release];
}

void WindowEvents::FlipBuffers()
{
    if (!initialized) return;
	[render_context->GetPimpl()->GetContextObject() flushBuffer];
}


void WindowEvents::MakeCurrentDraw(RenderContext * render_context)
{
	if (!initialized) return;
	NSOpenGLContext * context = render_context->GetPimpl()->GetContextObject();
	[context makeCurrentContext];
	[context setView:[this->data->cocoa contentView]]; //Needs auto release
}



void WindowEvents::SetRenderContext(RenderContext * in_render_context)
{
	if (!initialized) return;
	if (render_context)
	{
		std::cout << "Context already exists for this window." << std::endl;
		throw 1;
	}

	render_context = in_render_context;
	
}

bool WindowEvents::Open()
{
	if (initialized) return false;
	
    if (this->data)
	{
		delete this->data;
	}
	this->data = new WindowData();	
    
	if (window_count == 0)
	{
		master_pool = [[NSAutoreleasePool alloc] init];
	}

	NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];

	
	if (!application_launched)
	{
		[NSApplication sharedApplication];
	}
	
    //Setup Window
    NSRect rect = NSMakeRect (0, 0, width, height);
	
    unsigned int styleMask = 0;
    styleMask |= NSTitledWindowMask;
    styleMask |= NSClosableWindowMask;
    styleMask |= NSMiniaturizableWindowMask;
	styleMask |= NSResizableWindowMask;
	
	
    this->data->cocoa = [[WindowData_Cocoa alloc] InitWithContentRect: rect
										 StyleMask: styleMask
									  WindowEvents: this];
	
    [this->data->cocoa setAcceptsMouseMovedEvents: YES];
	
    [this->data->cocoa setTitle: @"Version: MacOSX Window"];
	
    RightMouseView * rm_view = [[RightMouseView alloc] initWithFrame: rect];
    [this->data->cocoa setContentView: rm_view];
	this->data->rm_view = rm_view;
	//[this->data->cocoa setContentView: this->data->cocoa];
	
    WindowDelegate * window_delegate = [[WindowDelegate alloc] init];
    [this->data->cocoa setDelegate: window_delegate];
	this->data->delegate = window_delegate;
	
    if (!application_launched)
    {
        //Setup Menu
		NSMenu * menu = [NSMenu new];
        [menu addItemWithTitle: @"Quit" action: @selector (terminate:) keyEquivalent: @"q"];
        [NSApp setMainMenu: menu];
		[menu release];
		[NSApp finishLaunching];
		application_launched = true;
    }
    
	window_count += 1;
	if (window_count == 1)
    {
		//First time constructed
		InitKeyMap();
    }
	initialized = true;
	this->MakeActive();
	
	//Send resize event.
	InputEvent input_event;
	input_event.SetType(InputEvent::RESIZE);
	input_event.data.resize.SetWidth(width);
	input_event.data.resize.SetHeight(height);
	GetInputManager().PushEvent(input_event);
	
	[pool drain];
	
	return true;
}

void WindowEvents::SetVSync(bool sync)
{
	if (!render_context)
	{
		std::cout << "Render Context Needed To Set Vertical Sync Mode." << std::endl;
		throw 1;
	}
	int sync_value = sync ? 1 : 0;
	[render_context->GetPimpl()->GetContextObject() setValues:&sync_value forParameter: NSOpenGLCPSwapInterval];
}

void WindowEvents::Configure(bool maximizable, bool minimizable, bool resizable)
{
	if (!initialized) return;

	this->maximizable = maximizable;
	this->minimizable = minimizable;
	this->resizable = resizable;
	
	unsigned int old_style = [data->cocoa styleMask];
	
	unsigned int style_mask = NSClosableWindowMask;
	if (old_style & NSBorderlessWindowMask == 0)
	{
		style_mask |= NSTitledWindowMask;
	}
	else
	{
		style_mask |= NSBorderlessWindowMask;
	}


	if (maximizable)
	{
		style_mask |= NSResizableWindowMask;
	}
	if (minimizable)
	{
		style_mask |= NSMiniaturizableWindowMask;
	}
	if (resizable)
	{
		style_mask |= NSResizableWindowMask;
	}
	
	[data->cocoa setStyleMask:style_mask];

	visible ? Show() : Hide();
}


void WindowEvents::SetAlwaysOnTop(bool top_most)
{
	if (!initialized) return;

	if (top_most)
	{
		[data->cocoa setLevel: CGShieldingWindowLevel()];
	}
	else
	{
		[data->cocoa setLevel: NSNormalWindowLevel];
	}
}

void WindowEvents::SetShowTitleBar(bool show)
{
	if (!initialized) return;

	unsigned int style_mask = 0;
	if (show)
	{
		style_mask |= NSTitledWindowMask | NSClosableWindowMask;
		
		if (maximizable)
		{
			style_mask |= NSResizableWindowMask;
		}
		if (minimizable)
		{
			style_mask |= NSMiniaturizableWindowMask;
		}
		if (resizable)
		{
			style_mask |= NSResizableWindowMask;
		}
	}
	else
	{
		style_mask = NSBorderlessWindowMask;
		
	}
	[data->cocoa setStyleMask:style_mask];

	visible ? Show() : Hide();
}

bool WindowEvents::TitleBarVisible() const
{
	if (!initialized) return false;
	
	return [data->cocoa styleMask] != NSBorderlessWindowMask;
}

void WindowEvents::SetTopLeftPosition(int x, int y)
{
	if (!initialized) return;
	
	int screen_height = CGDisplayBounds(kCGDirectMainDisplay).size.height;
	[data->cocoa setFrameTopLeftPoint: NSMakePoint(x, screen_height - y)];
}

void WindowEvents::GetTopLeftPosition(int & x, int & y) const
{
	if (!initialized) return;
	
	NSRect frame_rect = [data->cocoa frame]; //Frame for entire window.
	x = frame_rect.origin.x;
	int screen_height = CGDisplayBounds(kCGDirectMainDisplay).size.height;
	y = screen_height - (frame_rect.origin.y + frame_rect.size.height);
}

void WindowEvents::ScreenToClientCoordinates(int screen_x, int screen_y, int & rtn_client_x, int & rtn_client_y) const
{
	if (!initialized) return;
	
	
	NSRect frame_rect = [data->cocoa frame]; //Frame for entire window.
	NSRect content_rect = [[data->cocoa contentView] frame]; //Frame for just content inside window.
	int screen_height = CGDisplayBounds(kCGDirectMainDisplay).size.height;
	
    rtn_client_x = screen_x - frame_rect.origin.x;
	rtn_client_y = content_rect.size.height + screen_y + frame_rect.origin.y - screen_height;
    
}

void WindowEvents::ClientToScreenCoordinates(int client_x, int client_y, int & rtn_screen_x, int & rtn_screen_y) const
{
	if (!initialized) return;
	
	NSRect frame_rect = [data->cocoa frame]; //Frame for entire window.
	NSRect content_rect = [[data->cocoa contentView] frame]; //Frame for just content inside window.
	int screen_height = CGDisplayBounds(kCGDirectMainDisplay).size.height;

    rtn_screen_x = client_x + (frame_rect.origin.x);
    rtn_screen_y = screen_height - ((content_rect.size.height - client_y) + frame_rect.origin.y);
    
}

void WindowEvents::SetClipMouseToClientArea(bool clip)
{
	if (!initialized) return;
	
	//Use NSView Mouse Tracking Area. addTrackingRect:owner:userData:assumeInside:
	
	if (clip)
	{
		NSView * view = [data->cocoa contentView];
		NSRect content_rect = [view frame];
		[[data->cocoa contentView] addTrackingRect:content_rect owner:view userData:NULL assumeInside:YES];
		
	}
	else
	{
		
		[[data->cocoa contentView] removeTrackingRect];

	}
}

void WindowEvents::GetClientTopLeft(int & x, int & y) const
{
	NSRect frame_rect = [data->cocoa frame]; //Frame for entire window.
	NSRect content_rect = [[data->cocoa contentView] frame]; //Frame for just content inside window. W.r.t window origin.
	x = frame_rect.origin.x;
	int screen_height = CGDisplayBounds(kCGDirectMainDisplay).size.height;
	y = screen_height - (frame_rect.origin.y + content_rect.origin.y + content_rect.size.height);
}
void WindowEvents::GetFrameWidths(int & left, int & bottom, int & right, int & top) const
{
	NSRect frame_rect = [data->cocoa frame]; //Frame for entire window.
	NSRect content_rect = [[data->cocoa contentView] frame]; //Frame for just content inside window.
	
	int total_width = frame_rect.size.width;
	int total_height = frame_rect.size.height;
	left = content_rect.origin.x;
	bottom = content_rect.origin.y;
	right = total_width - content_rect.size.width - left;
	top = total_height - content_rect.size.height - bottom;
}
void WindowEvents::GetClientDimensions(unsigned int & w, unsigned int & h) const
{
	NSRect content_rect = [[data->cocoa contentView] frame]; //Frame for just content inside window.
	w = content_rect.size.width;
	h = content_rect.size.height;
}

