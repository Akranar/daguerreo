#include "../WindowEvents.h"
#include "../RenderContext.h"
#include "RenderContextPimpl_LINUX.h"
#include "WindowData_LINUX.h"
#include "KeyMapping_LINUX.h"
#include "DisplayModeManagerPimpl_LINUX.h"

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>
#include <GL/glext.h>
#include <GL/glxext.h>
#include <iostream>
#include <cstring>


/*
	Relevant Links and References:

		Overview:
			-> http://en.wikipedia.org/wiki/X_Window_System_protocols_and_architecture

		XInput2:
			-> http://who-t.blogspot.com/2009/05/xi2-recipes-part-1.html

		XEventsQueued:
			-> http://tronche.com/gui/x/xlib/event-handling/XEventsQueued.html

		GLX Reference:
			-> http://glprogramming.com/blue/ch07.html

		Window Functions:
			-> http://tronche.com/gui/x/xlib/window/

			XDestroyWindow(display, w)
				-> Display *display;
				-> Window w;
				-> http://tronche.com/gui/x/xlib/window/XDestroyWindow.html

		Window Focus:
			-> http://tronche.com/gui/x/xlib/input/XGetInputFocus.html

		Event Structures:
			-> http://tronche.com/gui/x/xlib/events/structures.html#XEvent

		XLookupKeysym:
			-> http://tronche.com/gui/x/xlib/utilities/keyboard/XLookupKeysym.html

		Fullscreen:
			-> http://tonyobryan.com/index.php?article=9
			-> XChangeProperty
				-> http://tronche.com/gui/x/xlib/window-information/XChangeProperty.html

			-> NEED?
				-> libxxf86vm-dev
					-> sudo apt-get install libxxf86vm-dev

				-> I might as well list OpenGL here too:
					-> libgl1-mesa-dev

			-> Bool XF86VidModeGetAllModeLines(Display *display, int screen, int *modecount_return, XF86VidModeModeInfo ***modesinfo);

			-> http://www.gamedev.net/topic/165191-linuxxf86-problem/

            Decorations:
                -> http://stackoverflow.com/questions/1904445/borderless-windows-on-linux

                struct MwmHints {
                    unsigned long flags;
                    unsigned long functions;
                    unsigned long decorations;
                    long input_mode;
                    unsigned long status;
                };
                enum {
                    MWM_HINTS_FUNCTIONS = (1L << 0),
                    MWM_HINTS_DECORATIONS =  (1L << 1),

                    MWM_FUNC_ALL = (1L << 0),
                    MWM_FUNC_RESIZE = (1L << 1),
                    MWM_FUNC_MOVE = (1L << 2),
                    MWM_FUNC_MINIMIZE = (1L << 3),
                    MWM_FUNC_MAXIMIZE = (1L << 4),
                    MWM_FUNC_CLOSE = (1L << 5)
                };

        Minimize Window (Iconifiy):
            -> http://tronche.com/gui/x/xlib/ICC/client-to-window-manager/XIconifyWindow.html

        Maximize Window:
            -> Send ClientMessage
                -> http://standards.freedesktop.org/wm-spec/1.3/ar01s05.html




	Notes:
		All Windows In Hierarchy.
			-> Top-Level Windows
				-> Child of root window.
		Window Attributes:
			-> Size, position, background

*/


//http://people.gnome.org/~tthurman/docs/metacity/xprops_8h-source.html

struct Hints
{
	unsigned long flags;
	unsigned long functions;
	unsigned long decorations;
	long inputMode;
	unsigned long status;
};

#define MWM_HINTS_FUNCTIONS     (1L << 0)
#define MWM_HINTS_DECORATIONS   (1L << 1)
#define MWM_HINTS_INPUT_MODE    (1L << 2)
#define MWM_HINTS_STATUS        (1L << 3)

#define MWM_FUNC_ALL            (1L << 0)
#define MWM_FUNC_RESIZE         (1L << 1)
#define MWM_FUNC_MOVE           (1L << 2)
#define MWM_FUNC_MINIMIZE       (1L << 3)
#define MWM_FUNC_MAXIMIZE       (1L << 4)
#define MWM_FUNC_CLOSE          (1L << 5)

#define MWM_DECOR_ALL           (1L << 0)
#define MWM_DECOR_BORDER        (1L << 1)
#define MWM_DECOR_RESIZEH       (1L << 2)
#define MWM_DECOR_TITLE         (1L << 3)
#define MWM_DECOR_MENU          (1L << 4)
#define MWM_DECOR_MINIMIZE      (1L << 5)
#define MWM_DECOR_MAXIMIZE      (1L << 6)

#define MWM_INPUT_MODELESS 0
#define MWM_INPUT_PRIMARY_APPLICATION_MODAL 1
#define MWM_INPUT_SYSTEM_MODAL 2
#define MWM_INPUT_FULL_APPLICATION_MODAL 3
#define MWM_INPUT_APPLICATION_MODAL MWM_INPUT_PRIMARY_APPLICATION_MODAL

#define MWM_TEAROFF_WINDOW      (1L<<0)


Atom SkipMouse;
int skip_mouse_count = 0;
int WindowEvents::window_count = 0;
int windowevents_instance_count = 0;


#define PushInputEvent(input_event) WindowEvents::GetActive()->GetInputManager().PushEvent(input_event);


//Constructors
WindowEvents::WindowEvents()
:
width(800),
height(600),
auto_repeat(false),
initialized(false),
fullscreen(false),
render_context(0),
data(0),
maximizable(true),
minimizable(true),
resizable(true)
{
	MakeActive();
	windowevents_instance_count += 1;
	if (windowevents_instance_count == 1)
	{
	    WindowData::OpenDisplay();
	}
};

WindowEvents::WindowEvents(int width, int height)
:
width(width),
height(height),
auto_repeat(false),
initialized(false),
fullscreen(false),
render_context(0),
data(0),
maximizable(true),
minimizable(true),
resizable(true)
{
	MakeActive();
	windowevents_instance_count += 1;
	if (windowevents_instance_count == 1)
	{
	    WindowData::OpenDisplay();
	}
}



//Destructor

WindowEvents::~WindowEvents()
{
	if (initialized) Close();
	if (data) delete data;

	windowevents_instance_count -= 1;
	if (windowevents_instance_count == 0 && DisplayModeManagerPimpl::NoManagersAlive())
	{
	    WindowData::CloseDisplay();
	}
};

//Static Methods
WindowEvents * WindowEvents::GetActive()
{
	return WindowData::active_window;
}

int WindowEvents::GetWindowCount()
{
	return window_count;
}


void WindowEvents::SetMousePosition(int x, int y)
{
	XEvent xevent;
	xevent.xclient.type = ClientMessage;
	xevent.xclient.message_type = SkipMouse;
	xevent.xclient.format = 32;
	xevent.xclient.data.l[0] = 0;

	//Send the ClientMessage event to Skip the next mouse event.
	XSendEvent(GetActive()->data->GetDisplay(), GetActive()->data->GetWindow(), 0, 0, &xevent);

    //Send the next event, which will warp the pointer
	XWarpPointer(GetActive()->data->GetDisplay(), None, GetActive()->data->GetWindow(), 0, 0, 0, 0, x, y);
	XFlush(GetActive()->data->GetDisplay());
}



void WindowEvents::SetShowMouse(bool show)
{
    if (!initialized) return;

	Display * display = data->GetDisplay();
	Window & window = data->GetWindow();
	bool cursor_shown = data->cursor_shown;

	if (show && !cursor_shown)
	{
		XUndefineCursor(display, window);
		XFlush(display);
		data->cursor_shown = true;
	}
	else if (!show && cursor_shown)
	{
		Pixmap bitmapNoData;
		XColor black;
		char noData[] = { 0,0,0,0,0,0,0,0 };
		black.red = black.green = black.blue = 0;

		bitmapNoData = XCreateBitmapFromData(display, window, noData, 8, 8);
		Cursor invisible_cursor = XCreatePixmapCursor(display, bitmapNoData, bitmapNoData, &black, &black, 0, 0);
		XFreePixmap(display, bitmapNoData);
		XDefineCursor(display, window, invisible_cursor);
		XFreeCursor(display, invisible_cursor);
		XFlush(display);
		data->cursor_shown = false;
	}

}


//Active Draw / Message Window
void WindowEvents::MakeActive()
{
    if (!initialized) return;
	WindowData::active_window = this;
}
void WindowEvents::MakeCurrentDraw()
{
	if (!initialized) return;
	glXMakeCurrent(data->GetDisplay(), data->GetWindow(), render_context->GetPimpl()->GetGLXContext());
}


//Full Screen and Size Methods
bool WindowEvents::EnterFullscreen()
{
	if (!initialized) return false;

	Display * display = data->GetDisplay();
    Window & window = data->GetWindow();
    Atom wm_state  =  XInternAtom(display, "_NET_WM_STATE", False);
    Atom state_fullscreen  =  XInternAtom(display, "_NET_WM_STATE_FULLSCREEN", False);

    XEvent xev;
    memset(&xev, 0, sizeof(xev));
    xev.type = ClientMessage;
    xev.xclient.window = window;
    xev.xclient.message_type = wm_state;
    xev.xclient.format = 32;

    xev.xclient.data.l[0] = 1; //NET_WM_STATE_ADD
    xev.xclient.data.l[1] = state_fullscreen;
    xev.xclient.data.l[2] = 0;

	XSendEvent(display, DefaultRootWindow(display), False, SubstructureNotifyMask, &xev);

	data->SetTitleVisible(false);
	return true;
}
void WindowEvents::ExitFullscreen()
{
	if (!initialized) return;

	Display * display = data->GetDisplay();
    Window & window = data->GetWindow();
    Atom wm_state  =  XInternAtom(display, "_NET_WM_STATE", False);
    Atom state_fullscreen  =  XInternAtom(display, "_NET_WM_STATE_FULLSCREEN", False);

    XEvent xev;
    memset(&xev, 0, sizeof(xev));
    xev.type = ClientMessage;
    xev.xclient.window = window;
    xev.xclient.message_type = wm_state;
    xev.xclient.format = 32;

    xev.xclient.data.l[0] = 0; //NET_WM_STATE_REMOVE
    xev.xclient.data.l[1] = state_fullscreen;
    xev.xclient.data.l[2] = 0;

	XSendEvent(display, DefaultRootWindow(display), False, SubstructureNotifyMask, &xev);

	data->SetTitleVisible(true);
}

bool WindowEvents::IsMaximized()
{
    Display * display = data->GetDisplay();
    Window & window = data->GetWindow();

    Atom wm_state  =  XInternAtom(display, "_NET_WM_STATE", False);
    Atom max_horz  =  XInternAtom(display, "_NET_WM_STATE_MAXIMIZED_HORZ", False);
    Atom max_vert  =  XInternAtom(display, "_NET_WM_STATE_MAXIMIZED_VERT", False);

    //Check Property for max_horz and max_vert
    //  -> Both present means window is maximized.

    Atom type;
    int format;
    unsigned long item_count, bytes_after;
    unsigned char * properties = NULL;
    XGetWindowProperty(display, window, wm_state, 0, (~0L), False, AnyPropertyType, &type, &format, &item_count, &bytes_after, &properties);

    bool max_horz_found = false;
    bool max_vert_found = false;

    if (format == 32)
    {
        for (unsigned int i = 0; i < item_count; ++i)
        {
            long data = ((long *)(properties))[i];
            if (data == max_horz) max_horz_found = true;
            if (data == max_vert) max_vert_found = true;
        }
    }
    else if (format == 16)
    {
        for (unsigned int i = 0; i < item_count; ++i)
        {
            short data = ((short *)(properties))[i];
            if (data == max_horz) max_horz_found = true;
            if (data == max_vert) max_vert_found = true;
        }
    }
    else if (format == 8)
    {
        for (unsigned int i = 0; i < item_count; ++i)
        {
            char data = ((char *)(properties))[i];
            if (data == max_horz) max_horz_found = true;
            if (data == max_vert) max_vert_found = true;
        }
    }

    return max_horz_found && max_vert_found;
}

void WindowEvents::ToggleMaximize()
{
    /*
        http://standards.freedesktop.org/wm-spec/1.3/ar01s05.html

        To change the state of a mapped window, a Client MUST send a _NET_WM_STATE client message to the root window
            -> window is the respective window
            -> type _NET_WM_STATE
            -> format 32
            -> l[0]=<the action, as listed below>,
            -> l[1]=<First property to alter>,
            -> l[2]=<Second property to alter>.

          This message allows two properties to be changed simultaneously, specifically to allow both horizontal and vertical maximisation to be altered together.
                -> l[2] MUST be set to zero if only one property is to be changed.
                -> l[0], the action, MUST be one of:

          _NET_WM_STATE_REMOVE        0    // remove/unset property
          _NET_WM_STATE_ADD           1    // add/set property
          _NET_WM_STATE_TOGGLE        2    // toggle property
    */
    Display * display = data->GetDisplay();
    Window & window = data->GetWindow();

    Atom wm_state  =  XInternAtom(display, "_NET_WM_STATE", False);
    Atom max_horz  =  XInternAtom(display, "_NET_WM_STATE_MAXIMIZED_HORZ", False);
    Atom max_vert  =  XInternAtom(display, "_NET_WM_STATE_MAXIMIZED_VERT", False);

    XEvent xev;
    memset(&xev, 0, sizeof(xev));
    xev.type = ClientMessage;
    xev.xclient.window = window;
    xev.xclient.message_type = wm_state;
    xev.xclient.format = 32;

    xev.xclient.data.l[0] = 2; //NET_WM_STATE_TOGGLE
    xev.xclient.data.l[1] = max_horz;
    xev.xclient.data.l[2] = max_vert;

    XSendEvent(display, DefaultRootWindow(display), False, SubstructureNotifyMask, &xev);
}
void WindowEvents::Minimize()
{

    //Iconification:
    //  Link: http://tronche.com/gui/x/xlib/ICC/client-to-window-manager/XIconifyWindow.html
    //      -> Sends ClientMessage with:
    //        -> format = 32
    //        -> message_type = Atom for WM_CHANGE_STATE
    //        -> first data = IconicState


    Display * display = data->GetDisplay();
    Window & window = data->GetWindow();

    XEvent xev;
    Atom wm_change_state  =  XInternAtom(display, "WM_CHANGE_STATE", False);

    memset(&xev, 0, sizeof(xev));
    xev.type = ClientMessage;
    xev.xclient.window = window;
    xev.xclient.message_type = wm_change_state;
    xev.xclient.format = 32;
    xev.xclient.data.l[0] = IconicState;

    XSendEvent(display, DefaultRootWindow(display), False, SubstructureNotifyMask, &xev);
}

void WindowEvents::SetSize(int w, int h)
{
	XResizeWindow(data->GetDisplay(), data->GetWindow(), w, h);

	InputEvent resize_event;
	resize_event.SetType(InputEvent::RESIZE);
	resize_event.data.resize.SetWidth(w);
	resize_event.data.resize.SetHeight(h);
	GetInputManager().PushEvent(resize_event);

}

//Window Visibility
void WindowEvents::Show()
{
    if (!initialized) return;
	XMapWindow(this->data->GetDisplay(), this->data->GetWindow());
	XFlush(data->GetDisplay());
	visible = true;
}
void WindowEvents::Hide()
{
    if (!initialized) return;
	XUnmapWindow(this->data->GetDisplay(), this->data->GetWindow());
	XFlush(data->GetDisplay());
	false;
}

//Window Termination
void WindowEvents::Close()
{
    if (!initialized) return;
    if (fullscreen) ExitFullscreen();

    if (WindowData::active_window == this)
    {
        WindowData::active_window = 0;
    }
	XUnmapWindow(this->data->GetDisplay(), this->data->GetWindow());
	XDestroyWindow(this->data->GetDisplay(), this->data->GetWindow());
	XFlush(this->data->GetDisplay());

	delete data;
	data = 0;
	window_count -= 1;
	initialized = false;
}

void WindowEvents::SetTitle(const char * text)
{
    if (!initialized) return;
	XStoreName(this->data->GetDisplay(), this->data->GetWindow(), text);
}

void WindowEvents::FlipBuffers()
{
    if (!initialized) return;
    glXSwapBuffers(this->data->GetDisplay(), this->data->GetWindow());
    XFlush(this->data->GetDisplay());
}

//Static Message Processing
int WindowEvents::ProcessEvents()
{
    //std::cout << "PRCES" << std::endl;
	bool process_mouse_movement = true;
	WindowEvents * targeted_window;
    XEvent event_report;
	unsigned short key_repeated;
	//int event_count = XEventsQueued(WindowData::display, QueuedAfterReading);
	//for (int i = 0; i < event_count; ++i)
	while (XEventsQueued(WindowData::display, QueuedAfterReading))
    {
        XNextEvent(WindowData::display, &event_report);
		InputEvent input_event;

		switch (event_report.type)
		{
			case FocusIn:
			{
				if (event_report.xfocus.mode == NotifyNormal)
				{
				    //std::cout << "FOCUS IN" << std::endl;
				    targeted_window = WindowData::FindWindowEvents(event_report.xfocus.window);
				    if (targeted_window)
				    {
				        targeted_window->MakeActive();

				        InputEvent input_event;
                        input_event.SetType(InputEvent::WINDOW_ACTIVATION);
                        input_event.data.window_activation.SetActive(true);
                        PushInputEvent(input_event);

						Window root_return;
						Window child_return;
						int root_x_return;
						int root_y_return;
						int win_x_return;
						int win_y_return;
						unsigned int mask_return;
						XQueryPointer(event_report.xfocus.display, event_report.xfocus.window, &root_return, &child_return, &root_x_return, &root_y_return, &win_x_return, &win_y_return, &mask_return);

						input_event.SetType(InputEvent::MOUSE_MOTION);
						input_event.data.mouse_motion.SetX(win_x_return);
						input_event.data.mouse_motion.SetY(win_y_return);
						input_event.data.mouse_motion.SetRelX(0);
						input_event.data.mouse_motion.SetRelY(0);
						PushInputEvent(input_event);

						WindowData::active_window->data->prev_mousex = win_x_return;
						WindowData::active_window->data->prev_mousey = win_y_return;
					}
				}
				else
				{
                    //std::cout << "FOCUS IN ABNORMAL" << std::endl;
				}
			}
			continue;
			case FocusOut:
			{
			    event_report.xfocus.window;
				if (event_report.xfocus.mode == NotifyNormal)
				{
				    //std::cout << "FOCUS OUT" << std::endl;
				    targeted_window = WindowData::FindWindowEvents(event_report.xfocus.window);
                    if (targeted_window && targeted_window == WindowData::active_window)
                    {
                        InputEvent input_event;
                        input_event.SetType(InputEvent::WINDOW_ACTIVATION);
                        input_event.data.window_activation.SetActive(false);
                        targeted_window->GetInputManager().PushEvent(input_event);
                        WindowData::active_window = 0;
                    }
                    //std::cout << "wtf" << std::endl;
				}
				else
				{
                    //std::cout << "FOCUS OUT ABNORMAL" << std::endl;
				}
			}
			continue;
			case ConfigureNotify:
			{
			    //std::cout << "CONFIGURE NOTIFY" << std::endl;
				targeted_window = WindowData::FindWindowEvents(event_report.xclient.window);
				if (targeted_window)
				{
				    input_event.SetType(InputEvent::RESIZE);
                    input_event.data.resize.SetWidth(event_report.xconfigure.width);
                    input_event.data.resize.SetHeight(event_report.xconfigure.height);
                    targeted_window->GetInputManager().PushEvent(input_event);
				}
			}
			continue;
			case ClientMessage:
			{
			    //std::cout << "CLIENT MESSAGE" << std::endl;
				if (event_report.xclient.message_type == SkipMouse)
				{
					skip_mouse_count += 1;
				}
				else
				{
					targeted_window = WindowData::FindWindowEvents(event_report.xclient.window);
					if (targeted_window)
					{
					    //std::cout << "TERMINATE" << std::endl;
                        targeted_window->Close();

                        //Doesn't send Focus events, so determine it manually.
                        if (GetWindowCount())
                        {
                            Window focus_return;
                            int revert_to_return;
                            XGetInputFocus(WindowData::GetDisplay(), &focus_return, &revert_to_return);
                            WindowEvents * new_active = WindowData::FindWindowEvents(focus_return);
                            if (new_active)
                            {
                                new_active->MakeActive();
                            }
                        }
					}
				}
			}
			continue;
			default:
			break;
		}

		if (WindowData::active_window == 0) continue;

		WindowEvents * active_window = WindowData::active_window;

		switch (event_report.type)
		{
			case KeyRelease:
			{
				key_repeated = 0;
				if (XEventsQueued(active_window->data->GetDisplay(), QueuedAfterReading))
				{
					XEvent nev;
					XPeekEvent(active_window->data->GetDisplay(), &nev);

					if (nev.type == KeyPress && nev.xkey.time == event_report.xkey.time && nev.xkey.keycode == event_report.xkey.keycode)
					{
						if (active_window->AutoRepeat())
						{
							int keysym = XLookupKeysym(&nev.xkey, 0);
							KeyID::Key key;
							if (keysym & 0xff00)
							{
								key = KeyMap_LayoutIndep[keysym & 0xff];
							}
							else
							{
								key = KeyMap_LayoutDep[keysym];
							}
							input_event.SetType(InputEvent::KEYBOARD);
							input_event.data.key.SetType(KeyEvent::KEY_REPEAT);
							input_event.data.key.SetKey(key);
							PushInputEvent(input_event);
						}
						// delete retriggered KeyPress event
						XNextEvent (active_window->data->GetDisplay(), &event_report);
						key_repeated = 1;
					}
				}

				if (!key_repeated)
				{
					int keysym = XLookupKeysym(&event_report.xkey, 0);
					KeyID::Key key;
					if (keysym & 0xff00)
					{
						key = KeyMap_LayoutIndep[keysym & 0xff];
					}
					else
					{
						key = KeyMap_LayoutDep[keysym];
					}
					input_event.SetType(InputEvent::KEYBOARD);
					input_event.data.key.SetType(KeyEvent::KEY_UP);
					input_event.data.key.SetKey(key);
					PushInputEvent(input_event);
				}
			}
			break;
			case KeyPress:
			{
				int keysym = XLookupKeysym(&event_report.xkey, 0);
				KeyID::Key key;
				if (keysym & 0xff00)
				{
					key = KeyMap_LayoutIndep[keysym & 0xff];
				}
				else
				{
					key = KeyMap_LayoutDep[keysym];
				}
				input_event.SetType(InputEvent::KEYBOARD);
				input_event.data.key.SetType(KeyEvent::KEY_DOWN);
				input_event.data.key.SetKey(key);
				PushInputEvent(input_event);
			}
			break;
			case MotionNotify:
			{
				if (skip_mouse_count > 0)
				{
					skip_mouse_count -= 1;
					active_window->data->SetPrevMouseX(event_report.xmotion.x);
					active_window->data->SetPrevMouseY(event_report.xmotion.y);
					continue;
				}
				if (event_report.xmotion.window == GetActive()->data->GetWindow())
				{
					input_event.SetType(InputEvent::MOUSE_MOTION);
					input_event.data.mouse_motion.SetX(event_report.xmotion.x);
					input_event.data.mouse_motion.SetY(event_report.xmotion.y);
					input_event.data.mouse_motion.SetRelX(event_report.xmotion.x - WindowEvents::GetActive()->data->GetPrevMouseX());
					input_event.data.mouse_motion.SetRelY(event_report.xmotion.y - WindowEvents::GetActive()->data->GetPrevMouseY());
					PushInputEvent(input_event);

					active_window->data->SetPrevMouseX(event_report.xmotion.x);
					active_window->data->SetPrevMouseY(event_report.xmotion.y);
				}
			}
			break;
			case ButtonPress:
			{
					switch(event_report.xbutton.button)
					{
						case Button1:
							input_event.SetType(InputEvent::MOUSE_BUTTON);
							input_event.data.mouse_button.SetState(MouseButtonEvent::PRESSED);
							input_event.data.mouse_button.SetButton(MouseButtonEvent::BUTTON_LEFT);
							PushInputEvent(input_event);
							break;
						case Button2:
							input_event.SetType(InputEvent::MOUSE_BUTTON);
							input_event.data.mouse_button.SetState(MouseButtonEvent::PRESSED);
							input_event.data.mouse_button.SetButton(MouseButtonEvent::BUTTON_MIDDLE);
							PushInputEvent(input_event);
							break;
						case Button3:
							input_event.SetType(InputEvent::MOUSE_BUTTON);
							input_event.data.mouse_button.SetState(MouseButtonEvent::PRESSED);
							input_event.data.mouse_button.SetButton(MouseButtonEvent::BUTTON_RIGHT);
							PushInputEvent(input_event);
							break;
						case Button4:
							input_event.SetType(InputEvent::MOUSE_WHEEL);
							input_event.data.mouse_wheel.SetDirection(MouseWheelEvent::UP);
							PushInputEvent(input_event);
							break;
						case Button5:
							input_event.SetType(InputEvent::MOUSE_WHEEL);
							input_event.data.mouse_wheel.SetDirection(MouseWheelEvent::DOWN);
							PushInputEvent(input_event);
							break;
						default:
							break;
					}
			}
			break;
			case ButtonRelease:
			{
				switch(event_report.xbutton.button)
				{
					case Button1:
						input_event.SetType(InputEvent::MOUSE_BUTTON);
						input_event.data.mouse_button.SetState(MouseButtonEvent::RELEASED);
						input_event.data.mouse_button.SetButton(MouseButtonEvent::BUTTON_LEFT);
						PushInputEvent(input_event);
						break;
					case Button2:
						input_event.SetType(InputEvent::MOUSE_BUTTON);
						input_event.data.mouse_button.SetState(MouseButtonEvent::RELEASED);
						input_event.data.mouse_button.SetButton(MouseButtonEvent::BUTTON_MIDDLE);
						PushInputEvent(input_event);
						break;
					case Button3:
						input_event.SetType(InputEvent::MOUSE_BUTTON);
						input_event.data.mouse_button.SetState(MouseButtonEvent::RELEASED);
						input_event.data.mouse_button.SetButton(MouseButtonEvent::BUTTON_RIGHT);
						PushInputEvent(input_event);
						break;
					case Button4:
						break;
					case Button5:
						break;
					default:
						break;
				}
			}
			break;
		}
    }
	return GetWindowCount();
}

void WindowEvents::MakeCurrentDraw(RenderContext * render_context)
{
	if (!initialized) return;
	glXMakeCurrent(data->GetDisplay(), data->GetWindow(), render_context->GetPimpl()->GetGLXContext());
}



void WindowEvents::SetRenderContext(RenderContext * in_render_context)
{
	if (!initialized) return;
	if (render_context)
	{
		std::cout << "Context already exists for this window." << std::endl;
		throw 1;
	}

	data->RecreateWindow(in_render_context->GetPimpl()->GetGLXFBConfig());
	render_context = in_render_context;
}

bool WindowEvents::Open()
{

    if (initialized) return false;
	this->data = new WindowData(this);

	//Create Window
    Window created_window;

    if (render_context)
    {
        XVisualInfo *vi = glXGetVisualFromFBConfig(this->data->GetDisplay(), render_context->GetPimpl()->GetGLXFBConfig());
        XSetWindowAttributes swa;
        Colormap cmap;
        swa.colormap = cmap = XCreateColormap(this->data->GetDisplay(), RootWindow(this->data->GetDisplay(), vi->screen), vi->visual, AllocNone);
        swa.background_pixmap = None;
        swa.border_pixel = 0;
        swa.event_mask = StructureNotifyMask;
        created_window = XCreateWindow(this->data->GetDisplay(), DefaultRootWindow(data->GetDisplay()), 0, 0, this->width, this->height, 0, vi->depth, InputOutput, vi->visual, CWBorderPixel|CWColormap|CWEventMask, &swa);

        XFree(vi);
    }
    else
    {
        XSetWindowAttributes dummy_swa;
        created_window = XCreateWindow(this->data->GetDisplay(), DefaultRootWindow(data->GetDisplay()), 0, 0, this->width, this->height, 0, CopyFromParent, InputOutput, CopyFromParent, 0, &dummy_swa);
    }


	this->data->SetWindow(created_window);

	XStoreName(this->data->GetDisplay(), this->data->GetWindow(), "GL 3.0 Window");

	XSync(this->data->GetDisplay(), false);
	XFlush(this->data->GetDisplay());

	int input_flags = 0;
	input_flags |= PointerMotionMask;
	input_flags |= ExposureMask;
	input_flags |= KeyPressMask;
	input_flags |= KeyReleaseMask;
	input_flags |= ButtonPressMask;
	input_flags |= ButtonReleaseMask;
	input_flags |= StructureNotifyMask;
	input_flags |= FocusChangeMask;

	XSelectInput(this->data->GetDisplay(), this->data->GetWindow(), input_flags);

	Atom wmDeleteMessage = XInternAtom(this->data->GetDisplay(), "WM_DELETE_WINDOW", False);
	XSetWMProtocols(this->data->GetDisplay(), this->data->GetWindow(), &wmDeleteMessage, 1);

	XAutoRepeatOn(this->data->GetDisplay());

    //


	this->data->handle_map_window->insert(std::make_pair(this->data->GetWindow(), this));
	initialized = true;

	window_count += 1;
	if (window_count == 1)
	{
	    InitKeyMap();
	}

	InputEvent input_event;
	input_event.SetType(InputEvent::RESIZE);
	input_event.data.resize.SetWidth(this->GetWidth());
	input_event.data.resize.SetHeight(this->GetHeight());
	GetInputManager().PushEvent(input_event);


	return true;
}

void WindowEvents::SetVSync(bool sync)
{
    if (initialized)
    {
        data->SetVSync(sync);
    }
}


void WindowEvents::Configure(bool maximizable, bool minimizable, bool resizable)
{
	if (!initialized) return;

	this->maximizable = maximizable;
	this->minimizable = minimizable;
	this->resizable = resizable;

    //Have to use Motif Hints
    //  -> _NET_WM_ALLOWED_ACTIONS is Window Manager managed.

	Hints hints;
	hints.flags = 0;
	hints.flags |= MWM_HINTS_FUNCTIONS;
	hints.flags |= MWM_HINTS_DECORATIONS;
	hints.functions = MWM_FUNC_MOVE | MWM_FUNC_CLOSE;
	hints.decorations = MWM_DECOR_TITLE;

	if (maximizable)
	{
        hints.functions |= MWM_FUNC_MAXIMIZE;
        hints.decorations |= MWM_DECOR_MAXIMIZE;
	}
	if (minimizable)
	{
        hints.functions |= MWM_FUNC_MINIMIZE;
        hints.decorations |= MWM_DECOR_MINIMIZE;
	}
	if (resizable)
	{
        hints.functions |= MWM_FUNC_RESIZE ;
        hints.decorations |= MWM_DECOR_RESIZEH;
	}

	if (!TitleBarVisible())
	{
	    hints.decorations = 0;
	}


    Atom property = XInternAtom(this->data->GetDisplay(),"_MOTIF_WM_HINTS",True);
    XChangeProperty(this->data->GetDisplay(),this->data->GetWindow(),property,property,32,PropModeReplace,(unsigned char *)&hints,5);

	visible ? Show() : Hide();
}


void WindowEvents::SetAlwaysOnTop(bool top_most)
{
	if (!initialized) return;

	Display * display = data->GetDisplay();
    Window & window = data->GetWindow();
    Atom wm_state  =  XInternAtom(display, "_NET_WM_STATE", False);
    Atom state_above  =  XInternAtom(display, "_NET_WM_STATE_ABOVE", False);

    XEvent xev;
    memset(&xev, 0, sizeof(xev));
    xev.type = ClientMessage;
    xev.xclient.window = window;
    xev.xclient.message_type = wm_state;
    xev.xclient.format = 32;

    xev.xclient.data.l[1] = state_above;
    xev.xclient.data.l[2] = 0;
	if (top_most)
	{
        xev.xclient.data.l[0] = 1; //NET_WM_STATE_ADD
	}
	else
	{
        xev.xclient.data.l[0] = 0; //NET_WM_STATE_REMOVE
	}
	XSendEvent(display, DefaultRootWindow(display), False, SubstructureNotifyMask, &xev);
}

void WindowEvents::SetShowTitleBar(bool show)
{
	if (!initialized) return;

    Atom property = XInternAtom(this->data->GetDisplay(),"_MOTIF_WM_HINTS",True);
    Hints hints;
	if (show)
	{
        hints.flags = 0;
        hints.flags |= MWM_HINTS_FUNCTIONS;
        hints.flags |= MWM_HINTS_DECORATIONS;      // Specify that we're changing the window decorations.
        hints.functions = MWM_FUNC_MOVE | MWM_FUNC_CLOSE;
        hints.decorations = MWM_DECOR_TITLE;

        if (maximizable)
        {
            hints.functions |= MWM_FUNC_MAXIMIZE;
            hints.decorations |= MWM_DECOR_MAXIMIZE;
        }
        if (minimizable)
        {
            hints.functions |= MWM_FUNC_MINIMIZE;
            hints.decorations |= MWM_DECOR_MINIMIZE;
        }
        if (resizable)
        {
            hints.functions |= MWM_FUNC_RESIZE ;
            hints.decorations |= MWM_DECOR_RESIZEH;
        }

        if (XChangeProperty(this->data->GetDisplay(),this->data->GetWindow(),property,property,32,PropModeReplace,(unsigned char *)&hints,5))
        {
            data->SetTitleVisible(true);
        }
	}
	else
	{

        hints.flags = MWM_HINTS_DECORATIONS;  // Specify that we're changing the window decorations.
        hints.decorations = 0;  // 0 (false) means that window decorations should go bye-bye.

        if (XChangeProperty(this->data->GetDisplay(),this->data->GetWindow(),property,property,32,PropModeReplace,(unsigned char *)&hints,5))
        {
            data->SetTitleVisible(false);
        }
	}

	visible ? Show() : Hide();
}

bool WindowEvents::TitleBarVisible() const
{
	if (!initialized) return false;

	return data->GetTitleVisible();
}

void WindowEvents::SetTopLeftPosition(int x, int y)
{
	if (!initialized) return;
	XMoveWindow(data->GetDisplay(), data->GetWindow(), x, y);
}

void WindowEvents::GetTopLeftPosition(int & x, int & y) const
{
	if (!initialized) return;

	XWindowAttributes xwa;
    XGetWindowAttributes(data->GetDisplay(), data->GetWindow(), &xwa);

    int left_frame, bottom_frame, right_frame, top_frame;
    GetFrameWidths(left_frame, bottom_frame, right_frame, top_frame);

    x = xwa.x - left_frame;
    y = xwa.y - top_frame;
}

void WindowEvents::GetFrameWidths(int & left, int & bottom, int & right, int & top) const
{
    Display * display = data->GetDisplay();
    Window & window = data->GetWindow();

    Atom net_frame_extents  =  XInternAtom(display, "_NET_FRAME_EXTENTS", False);
    Atom type;
    int format;
    unsigned long item_count, bytes_after;
    unsigned char * properties = NULL;
    XGetWindowProperty(display, window, net_frame_extents, 0, (~0L), False, AnyPropertyType, &type, &format, &item_count, &bytes_after, &properties);

    if (format != 32)
    {
        std::cout << "Error in WindowEvents::GetFrameWidths : 32-bit format expected." << std::endl;
        throw 1;
    }
    if (item_count < 4)
    {
        std::cout << "Error in WindowEvents::GetFrameWidths : Expected 4 items." << std::endl;
        throw 1;
    }

    left = ((long *)(properties))[0];
    right = ((long *)(properties))[1];
    top = ((long *)(properties))[2];
    bottom = ((long *)(properties))[3];
}

void WindowEvents::GetClientTopLeft(int & x, int & y) const
{
	if (!initialized) return;

	XWindowAttributes xwa;
    XGetWindowAttributes(data->GetDisplay(), data->GetWindow(), &xwa);

    x = xwa.x;
    y = xwa.y;
}


void WindowEvents::ScreenToClientCoordinates(int screen_x, int screen_y, int & rtn_client_x, int & rtn_client_y) const
{
	if (!initialized) return;

    Window rtn_child_window;
	XTranslateCoordinates(data->GetDisplay(), DefaultRootWindow(data->GetDisplay()), data->GetWindow(), screen_x, screen_y, &rtn_client_x, &rtn_client_y, &rtn_child_window);
}

void WindowEvents::ClientToScreenCoordinates(int client_x, int client_y, int & rtn_screen_x, int & rtn_screen_y) const
{
	if (!initialized) return;
	Window rtn_child_window;
	XTranslateCoordinates(data->GetDisplay(), data->GetWindow(), DefaultRootWindow(data->GetDisplay()), client_x, client_y, &rtn_screen_x, &rtn_screen_y, &rtn_child_window);
}

void WindowEvents::SetClipMouseToClientArea(bool clip)
{
	if (!initialized) return;

	if (clip)
	{
        XGrabPointer(this->data->GetDisplay(),this->data->GetWindow(),True,0,GrabModeAsync,GrabModeAsync,this->data->GetWindow(),0L,CurrentTime);
	}
	else
	{
        XUngrabPointer(this->data->GetDisplay(), CurrentTime);
	}
}

void WindowEvents::GetClientDimensions(unsigned int & w, unsigned int & h) const
{
    if (!initialized) return;

	Window root_return;
	int x_return, y_return;
	unsigned int border_width_return;
	unsigned int depth_return;
    XGetGeometry(data->GetDisplay(), data->GetWindow(), &root_return, &x_return, &y_return, &w, &h, &border_width_return, &depth_return);

}


