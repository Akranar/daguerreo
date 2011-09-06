#include "WindowData_LINUX.h"
#include "../WindowEvents.h"

#include <cstring>
#include <iostream>
#include <float.h>
#include <complex>



Display * WindowData::display = 0;
std::map<Window, WindowEvents *> * WindowData::handle_map_window = 0;
WindowEvents * WindowData::active_window = 0;
PFNGLXSWAPINTERVALSGIPROC WindowData::glXSwapIntervalSGI = 0;


WindowData::WindowData(WindowEvents * window_events)
:
prev_mousex(0),
prev_mousey(0),
cursor_shown(true),
window_events(window_events),
title_bar_visible(true),
title_bar_height(0)
{
	fullscreen_mode = -1;
	if (!handle_map_window)
	{
		handle_map_window = new std::map<Window, WindowEvents *>;
	}
}

WindowData::~WindowData()
{
	handle_map_window->erase(GetWindow());
	if (handle_map_window->empty())
	{
	    //Note:
		//	-> Make sure to properly clean up.
		//	-> Constructing these resources a second time requires they be at their uninitialized values.
		delete handle_map_window;
        handle_map_window = 0;
	}
}



void WindowData::RecreateWindow(const GLXFBConfig & glx_fbconfig)
{

    handle_map_window->erase(GetWindow());
    XUnmapWindow(GetDisplay(), GetWindow());
	XDestroyWindow(GetDisplay(), GetWindow());


    XVisualInfo *vi = glXGetVisualFromFBConfig(GetDisplay(), glx_fbconfig);

    XSetWindowAttributes dummy_swa;
	XSetWindowAttributes swa;
	Colormap cmap;
	swa.colormap = cmap = XCreateColormap(GetDisplay(), RootWindow(GetDisplay(), vi->screen), vi->visual, AllocNone);
	swa.background_pixmap = None;
	swa.border_pixel = 0;
	swa.event_mask = StructureNotifyMask;

	Window created_window = XCreateWindow(GetDisplay(), DefaultRootWindow(GetDisplay()), 0, 0, window_events->GetWidth(), window_events->GetHeight(), 0, vi->depth, InputOutput, vi->visual, CWBorderPixel|CWColormap|CWEventMask, &swa);

	SetWindow(created_window);

	XFree(vi);
	XStoreName(GetDisplay(), GetWindow(), "GL 3.0 Window");

	XSync(GetDisplay(), false);
	XFlush(GetDisplay());

	int input_flags = 0;
	input_flags |= PointerMotionMask;
	input_flags |= ExposureMask;
	input_flags |= KeyPressMask;
	input_flags |= KeyReleaseMask;
	input_flags |= ButtonPressMask;
	input_flags |= ButtonReleaseMask;
	input_flags |= StructureNotifyMask;
	input_flags |= FocusChangeMask;

	XSelectInput(GetDisplay(), GetWindow(), input_flags);

	Atom wmDeleteMessage = XInternAtom(GetDisplay(), "WM_DELETE_WINDOW", False);
	XSetWMProtocols(GetDisplay(), GetWindow(), &wmDeleteMessage, 1);

    handle_map_window->insert(std::make_pair(GetWindow(), window_events));
    window_events->MakeActive();
    window_events->Configure(window_events->Maximizable(), window_events->Minimizable(), window_events->Resizable());
}



WindowEvents * WindowData::FindWindowEvents(Window & win)
{
	std::map<Window, WindowEvents *>::iterator it = handle_map_window->find(win);
	if (it == handle_map_window->end())
	{
		return 0;
	}
	return it->second;
}


void WindowData::OpenDisplay()
{
	if (!display)
	{
		display = XOpenDisplay(NULL);
	}
}

void WindowData::CloseDisplay()
{
	if (display)
	{
		XCloseDisplay(GetDisplay());
		display = 0;
	}
}


static bool IsExtensionSupported(const char *extList, const char *extension)
{
	const char *start;
	const char *where, *terminator;

	/* Extension names should not have spaces. */
	where = strchr(extension, ' ');
	if ( where || *extension == '\0' )
		return false;

	/* It takes a bit of care to be fool-proof about parsing the
	OpenGL extensions string. Don't be fooled by sub-strings,
	etc. */
	for (start = extList; ;)
	{
		where = strstr(start, extension);

		if ( !where )
			break;

		terminator = where + strlen(extension);

		if ( where == start || *(where - 1) == ' ' )
			if ( *terminator == ' ' || *terminator == '\0' )
				return true;

		start = terminator;
	}

	return false;
}

void WindowData::SetVSync(bool sync)
{
    const char *glx_extentions = glXQueryExtensionsString(GetDisplay(), DefaultScreen(GetDisplay()));

    if (!glXSwapIntervalSGI)
    {
        glXSwapIntervalSGI = (PFNGLXSWAPINTERVALSGIPROC) glXGetProcAddressARB( (const GLubyte *) "glXSwapIntervalSGI");
    }

    if (IsExtensionSupported(glx_extentions, "GLX_SGI_swap_control") && glXSwapIntervalSGI)
    {
        glXSwapIntervalSGI(sync ? 1 : 0);
    }
}

bool WindowData::NoWindowsAlive()
{
    return handle_map_window == 0;

}
