
#include "WindowData_DIRECTX9.h"
#include <iostream>

/*
#include <GL/gl.h>
#define GL_GLEXT_PROTOTYPES 1
#include <GL/glext.h>
#define WGL_WGLEXT_PROTOTYPES 1
#include "wglext.h" //downloaded from http://www.opengl.org/registry/api/wglext.h
*/

std::map<HWND, WindowEvents *> * WindowData::handle_map_window = 0;
int WindowData::vsync = 1;

WindowData::WindowData()
:
mouse_position_must_update(true)
{
	if (!handle_map_window)
	{
		handle_map_window = new std::map<HWND, WindowEvents *>;
	}

}

WindowData::~WindowData()
{
	handle_map_window->erase(handle_window);
	if (handle_map_window->empty())
	{
		delete handle_map_window;
		handle_map_window = 0;
	}
}

WindowEvents * WindowData::FindWindowEvents(HWND & hWnd)
{
	std::map<HWND, WindowEvents *>::iterator it = handle_map_window->find(hWnd);
	if (it == handle_map_window->end())
	{
		return 0;
	}
	return it->second;
}

static bool IsExtensionSupported(const char * extension_list, const char * extension)
{
	/*
	const char *start;
	const char *location, *terminator;

	//Extension names should not have spaces.
	location = strchr(extension, ' ');
	if (location || *extension == '\0') return false;

	//It takes a bit of care to be fool-proof about parsing the OpenGL extensions string. Don't be fooled by sub-strings, etc.
	for (start = extension_list; ;)
	{
		location = strstr(start, extension);
		if (!location) break;
		terminator = location + strlen(extension);
		if (location == start || *(location - 1) == ' ')
		{
			if (*terminator == ' ' || *terminator == '\0') return true;
		}
		start = terminator;
	}

	*/
	return false;
}

/*
	VSync:
		-> http://www.opengl.org/discussion_boards/ubbthreads.php?ubb=showflat&Number=271567
			-> With GLX (X11), uses extension GLX_SGI_video_sync
			-> With Mac (CGL, Carbon and Cocoa), set swap internal to a none zero value:
				-> http://developer.apple.com/mac/library/d...1987-CH216-SW12
			-> With WGL (Windows), use extension WGL_EXT_swap_control
*/
/*
void WindowData::SetVSync(bool sync)
{
	
	typedef bool (APIENTRY *PFNWGLSWAPINTERVALFARPROC)(int);

	PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT = 0;

	PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB = (PFNWGLGETEXTENSIONSSTRINGARBPROC) wglGetProcAddress("wglGetExtensionsStringARB");
	if (wglGetExtensionsStringARB)
	{
		const char * extension_string = wglGetExtensionsStringARB(this->GetDeviceContext());

		if (IsExtensionSupported(extension_string, "WGL_EXT_swap_control"))
		{
			wglSwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC)wglGetProcAddress("wglSwapIntervalEXT");
			wglSwapIntervalEXT(sync);
		}
	}
	
}
*/
