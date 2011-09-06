#include "../WindowEvents.h"

#include <windows.h>
#include <GL/gl.h>
#include <map>
#include <iostream>

#include "WindowData_WINDOWS.h"
#include "../RenderContext.h"
#include "RenderContextPimpl_WINDOWS.h"

/*
	Relevant Links and References:

		Window Functions:
			-> SetWindowLongPtr()
			-> http://msdn.microsoft.com/en-us/library/ff468919%28v=VS.85%29.aspx

		Changing Window Styles:
			-> http://msdn.microsoft.com/en-us/library/ms644898%28v=vs.85%29.aspx

		Change Window Position And Size:
			-> SetWindowPos()
			-> http://msdn.microsoft.com/en-us/library/ms633545%28v=vs.85%29.aspx

		Show Window:
			-> For Minimize, Maximize, Show, Hide
			-> http://msdn.microsoft.com/en-us/library/ms633548%28v=vs.85%29.aspx

		Using Keyboard Input:
				-> Gives Info on:
					-> Processing Keystroke Messages
					-> Translating Character Messages
					-> Processing Character Messages
					-> Using the Caret
					-> Displaying Keyboard Input
			-> http://msdn.microsoft.com/en-us/library/ms646268%28v=VS.85%29.aspx

		Raw Input Functions:
			-> http://msdn.microsoft.com/en-us/library/ff468896%28v=VS.85%29.aspx

		Raw Input Structures:
			-> http://msdn.microsoft.com/en-us/library/ff468899%28v=VS.85%29.aspx

        OpenGL FAQ:
            http://www.mechcore.net/w/OpenGL_FAQ

*/

//Global Variables

//Raw Device
//	-> 0 : Mouse
//	-> 1 : Keyboard
RAWINPUTDEVICE Rid[2];
#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC         ((USHORT) 0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE        ((USHORT) 0x02)
#endif

//Active WindowEvents and Pimpl, WindowData
WindowEvents * active_window = 0;
WindowData * active_data = 0;

//WindowEvents static initialization
int WindowEvents::window_count = 0;



//Constructors
WindowEvents::WindowEvents() 
: 
width(800), 
height(600), 
auto_repeat(false),
initialized(false),
fullscreen(false),
render_context(0),
data(0)
{
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
data(0)
{
	MakeActive();
}

//Destructor

WindowEvents::~WindowEvents()
{
	if (initialized) Close();
	if (data) delete data;
};


//Window Termination
void WindowEvents::Close()
{
	if (!initialized) return;
	if (fullscreen) ExitFullscreen();
	DestroyWindow(data->GetWindowHandle());
	ReleaseDC(data->GetWindowHandle(), data->GetDeviceContext());

    delete data;
    data = 0;

	initialized = false;
	window_count -= 1;	
}

//Window Sizing Methods
#include "WindowEvents_WINDOWS_Sizing.h"

//Event Handling
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
#define PushInputEvent(input_event) active_window->GetInputManager().PushEvent(input_event);
#include "KeyMapping_WINDOWS.h"
#include "WindowEvents_WINDOWS_ProcessEvents.h"

//Static Methods
WindowEvents * WindowEvents::GetActive()
{
	return active_window;
}

int WindowEvents::GetWindowCount()
{
	return window_count;
}

//Cursor Methods
void WindowEvents::SetMousePosition(int x, int y)
{
	POINT point;
	point.x = x;
	point.y = y;
	ClientToScreen(data->GetWindowHandle(), (LPPOINT) &point);
	SetCursorPos(point.x, point.y);
}

bool mouse_shown = true;
void WindowEvents::SetShowMouse(bool show)
{
	if (mouse_shown != show)
	{
		ShowCursor(show);
		mouse_shown = show;
	}
}

//Active Draw / Message Window
void WindowEvents::MakeActive()
{
	if (!initialized) return;

	active_window = this;
	active_data = this->data;

	Rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC; 
    Rid[0].usUsage = HID_USAGE_GENERIC_MOUSE; 
    Rid[0].dwFlags = RIDEV_INPUTSINK;  
	Rid[0].hwndTarget = this->data->handle_window;

	Rid[1].usUsagePage = HID_USAGE_PAGE_GENERIC; 
    Rid[1].usUsage = 0x06;
    Rid[1].dwFlags = RIDEV_INPUTSINK;  
	Rid[1].hwndTarget = this->data->handle_window;

	RegisterRawInputDevices(Rid, 2, sizeof(Rid[0]));
	
}
void WindowEvents::MakeCurrentDraw()
{
	if (!initialized) return;
	wglMakeCurrent(this->data->GetDeviceContext(), render_context->GetPimpl()->GetHGLRC());
}

void WindowEvents::MakeCurrentDraw(RenderContext * render_context)
{
	if (!initialized) return;
	wglMakeCurrent(this->data->GetDeviceContext(), render_context->GetPimpl()->GetHGLRC());
}

void WindowEvents::SetRenderContext(RenderContext * in_render_context)
{
	if (!initialized) return;
	if (render_context)
	{
		std::cout << "Context already exists for this window." << std::endl;
		throw 1;
	}

	PIXELFORMATDESCRIPTOR dummy;
	SetPixelFormat(data->GetDeviceContext(), in_render_context->GetPimpl()->GetPixelFormatId(), &dummy);
	render_context = in_render_context;
	
}

bool WindowEvents::Open()
{
	if (initialized) return false;
    this->data = new WindowData();
	
	HWND & hWnd = this->data->handle_window;
	HDC & hDC = this->data->handle_device_context;

	HINSTANCE hInstance = GetModuleHandle(NULL);

	//Init and register window class.
	WNDCLASSEX wcex;
	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= 0;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= TEXT("WindowClass");
	wcex.hIconSm		= 0;
	RegisterClassEx(&wcex);

	//Resize the window
	RECT rect = { 0, 0, width, height };
	AdjustWindowRect(&rect, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX, FALSE);

	//Create window from window class and get window handle.
	hWnd = CreateWindow( "WindowClass", 
						 "Window", 
						 WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME | CS_OWNDC,
						 CW_USEDEFAULT, 
						 CW_USEDEFAULT, 
						 rect.right - rect.left, 
						 rect.bottom - rect.top, 
						 NULL, 
						 NULL, 
						 hInstance, 
						 NULL);
   
	if (!hWnd) return false;

	//Get Device Context.
	hDC = GetDC(hWnd);

	//Update WindowEvents Book-keeping.
	this->data->handle_map_window->insert(std::make_pair(this->data->handle_window, this));
	window_count += 1;
	
	if (window_count == 1)
	{
		//First window
		InitKeyMap();
	}

	//Make window active.
	initialized = true;
	this->MakeActive();
	
	return true;
}

//Window Visibility
void WindowEvents::Show()
{
	if (!initialized) return;
	ShowWindow(this->data->GetWindowHandle(), SW_SHOW);
	UpdateWindow(this->data->GetWindowHandle());
	visible = true;
}
void WindowEvents::Hide()
{
	if (!initialized) return;
	ShowWindow(this->data->GetWindowHandle(), SW_HIDE);
	UpdateWindow(this->data->GetWindowHandle());
	visible = false;
}

void WindowEvents::SetTitle(const char * text)
{
	if (!initialized) return;
	SetWindowText(this->data->handle_window, text);
}

void WindowEvents::SetVSync(bool sync)
{
	if (!initialized) return;
	data->SetVSync(sync);
}

void WindowEvents::FlipBuffers()
{
	if (!initialized) return;
	SwapBuffers(this->data->handle_device_context);
}

void WindowEvents::Configure(bool maximizable, bool minimizable, bool resizable)
{
	if (!initialized) return;

	this->maximizable = maximizable;
	this->minimizable = minimizable;
	this->resizable = resizable;

	LONG_PTR config = WS_CAPTION | WS_SYSMENU;
	if (maximizable)
	{
		config |= WS_MAXIMIZEBOX;
	}
	if (minimizable)
	{
		config |= WS_MINIMIZEBOX;
	}
	if (resizable)
	{
		config |= WS_THICKFRAME;
	}
	SetWindowLongPtr(this->data->GetWindowHandle(), GWL_STYLE, config);
	SetWindowPos(this->data->GetWindowHandle(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
	visible ? Show() : Hide();
}


void WindowEvents::SetAlwaysOnTop(bool top_most)
{
	if (!initialized) return;

	if (top_most)
	{
		SetWindowPos(this->data->GetWindowHandle(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
	else
	{
		SetWindowPos(this->data->GetWindowHandle(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
}

void WindowEvents::SetShowTitleBar(bool show)
{
	if (!initialized) return;

	if (show)
	{
		LONG_PTR config = WS_CAPTION | WS_SYSMENU;
		if (maximizable)
		{
			config |= WS_MAXIMIZEBOX;
		}
		if (minimizable)
		{
			config |= WS_MINIMIZEBOX;
		}
		if (resizable)
		{
			config |= WS_THICKFRAME;
		}
		SetWindowLongPtr(this->data->GetWindowHandle(), GWL_STYLE, config);
	}
	else
	{
		SetWindowLongPtr(this->data->GetWindowHandle(), GWL_STYLE, 0);
	}

	visible ? Show() : Hide();
}

bool WindowEvents::TitleBarVisible() const
{
	LONG_PTR config = GetWindowLongPtr(this->data->GetWindowHandle(), GWL_STYLE);
	return (config & WS_CAPTION) != 0;
}

void WindowEvents::SetTopLeftPosition(int x, int y)
{
	SetWindowPos(this->data->GetWindowHandle(), 0, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

void WindowEvents::GetTopLeftPosition(int & x, int & y) const
{
	RECT window_rect;
	GetWindowRect(data->GetWindowHandle(), &window_rect);
	x = window_rect.left;
	y = window_rect.top;
}

void WindowEvents::ScreenToClientCoordinates(int screen_x, int screen_y, int & rtn_client_x, int & rtn_client_y) const
{
	POINT point;
	point.x = screen_x;
	point.y = screen_y;
	ScreenToClient(data->GetWindowHandle(), (LPPOINT) &point);
	rtn_client_x = point.x;
	rtn_client_y = point.y;
}

void WindowEvents::ClientToScreenCoordinates(int client_x, int client_y, int & rtn_screen_x, int & rtn_screen_y) const
{
	POINT point;
	point.x = client_x;
	point.y = client_y;
	ClientToScreen(data->GetWindowHandle(), (LPPOINT) &point);
	rtn_screen_x = point.x;
	rtn_screen_y = point.y;
}

void WindowEvents::SetClipMouseToClientArea(bool clip)
{
	if (clip)
	{
		RECT client_rect;
		GetClientRect(data->GetWindowHandle(), &client_rect);
		
		int top, left, right, bottom;
		ClientToScreenCoordinates(client_rect.left, client_rect.top, left, top);
		ClientToScreenCoordinates(client_rect.right, client_rect.bottom, right, bottom);

		RECT clip;
		clip.top = top;
		clip.left = left;
		clip.right = right;
		clip.bottom = bottom;
		
		ClipCursor(&clip);
	}
	else
	{
		ClipCursor(0);
	}
}

void WindowEvents::GetClientTopLeft(int & x, int & y) const
{
	RECT window_rect;
	GetWindowRect(data->GetWindowHandle(), &window_rect);

	int left, bottom, right, top;
	GetFrameWidths(left, bottom, right, top);

	x = window_rect.left + left;
	y = window_rect.top + top;
}
void WindowEvents::GetFrameWidths(int & left, int & bottom, int & right, int & top) const
{
	if (TitleBarVisible())
	{
		left = GetSystemMetrics(SM_CYBORDER);
		bottom = GetSystemMetrics(SM_CXBORDER);
		right = GetSystemMetrics(SM_CYBORDER);
		top = GetSystemMetrics(SM_CYCAPTION);
	}
	else
	{
		left = 0;
		bottom = 0;
		right = 0;
		top = 0;
	}

}
void WindowEvents::GetClientDimensions(unsigned int & w, unsigned int & h) const
{
	if (!initialized) return;
	RECT rect;
	GetClientRect(data->GetWindowHandle(), &rect);
	w = rect.right - rect.left;
	h = rect.bottom - rect.top;
}
