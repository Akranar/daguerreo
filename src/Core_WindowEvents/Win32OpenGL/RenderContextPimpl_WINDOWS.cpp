#include "RenderContextPimpl_WINDOWS.h"
#include "WindowData_WINDOWS.h"
#include "../WindowPixelFormat.h"
#include "../RenderContext.h"
#include <iostream>

PFNWGLGETEXTENSIONSSTRINGARBPROC RenderContextPimpl::wglGetExtensionsStringARB = 0;
PFNWGLCHOOSEPIXELFORMATARBPROC RenderContextPimpl::wglChoosePixelFormatARB = 0;
PFNWGLCREATECONTEXTATTRIBSARBPROC RenderContextPimpl::wglCreateContextAttribsARB = 0;


bool RenderContextPimpl::IsExtensionSupported(const char * extension_list, const char * extension)
{
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

	return false;
}

RenderContextPimpl::RenderContextPimpl(WindowData * window_data, const WindowPixelFormat * pixel_format)
{
	//int major_version = 4;
	//int minor_version = 0;

	//Most I can Support.

	//int major_version = 3;
	//int minor_version = 3;

	//int major_version = 3;
	//int minor_version = 1;

	//int major_version = 3;
	//int minor_version = 0;

	//Transform Feedback Added Here.

	//int major_version = 2;
	//int minor_version = 1;

	//int major_version = 2;
	//int minor_version = 0;

#ifdef USE_OPENGL32
	int major_version = 3;
	int minor_version = 2;
#elif USE_OPENGL21
	int major_version = 2;
	int minor_version = 1;
	//int major_version = 3;
	//int minor_version = 2;
#endif

	HDC false_hdc = 0;
	HWND false_hwnd = 0;
	int false_pixelformat_id = 0;
	HGLRC false_hglrc = 0;

	if (!wglGetCurrentDC())
	{
		MakeFalseWindowAndContext(false_hwnd, false_hdc, false_pixelformat_id, false_hglrc);
	}

	if (!wglGetExtensionsStringARB)
	{
		wglGetExtensionsStringARB = (PFNWGLGETEXTENSIONSSTRINGARBPROC) wglGetProcAddress("wglGetExtensionsStringARB");
		wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC) wglGetProcAddress("wglChoosePixelFormatARB");
		wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC) wglGetProcAddress("wglCreateContextAttribsARB");
	}

	int pixelformat_id;
	HGLRC hglrc;

	if (wglGetExtensionsStringARB) 
	{
		const char * extension_string = wglGetExtensionsStringARB(window_data->GetDeviceContext());
		if (IsExtensionSupported(extension_string, "WGL_ARB_pixel_format") && 
			IsExtensionSupported(extension_string, "WGL_ARB_create_context"))
		{
			SetAdvancedPixelFormat(window_data->GetWindowHandle(), window_data->GetDeviceContext(), pixel_format, pixelformat_id);
			CreateAdvancedContext(window_data->GetWindowHandle(), window_data->GetDeviceContext(), major_version, minor_version, hglrc);
		}
		else
		{
			SetBasicPixelFormat(window_data->GetWindowHandle(), window_data->GetDeviceContext(), pixel_format, pixelformat_id);
			CreateBasicContext(window_data->GetWindowHandle(), window_data->GetDeviceContext(), hglrc);
		}
	}
	else
	{
		SetBasicPixelFormat(window_data->GetWindowHandle(), window_data->GetDeviceContext(), pixel_format, pixelformat_id);
		CreateBasicContext(window_data->GetWindowHandle(), window_data->GetDeviceContext(), hglrc);
	}

	if (false_hwnd)
	{
		ReleaseDC(false_hwnd, false_hdc);
		DestroyWindow(false_hwnd);
		wglMakeCurrent(0, 0);
		wglDeleteContext(false_hglrc);
	}

	if (!hglrc)
	{
		std::cout << "Render Context Initialization Failed." << std::endl;
	}
	else
	{
		this->pixelformat_id = pixelformat_id;
		this->hgl_render_context = hglrc;
	}
}

RenderContextPimpl::RenderContextPimpl(WindowData * window_data, const RenderContext * render_context)
{
	int major_version = 3;
	int minor_version = 0;

	HDC false_hdc = 0;
	HWND false_hwnd = 0;
	int false_pixelformat_id = 0;
	HGLRC false_hglrc = 0;

	if (!wglGetCurrentDC())
	{
		MakeFalseWindowAndContext(false_hwnd, false_hdc, false_pixelformat_id, false_hglrc);
	}

	if (!wglGetExtensionsStringARB)
	{
		wglGetExtensionsStringARB = (PFNWGLGETEXTENSIONSSTRINGARBPROC) wglGetProcAddress("wglGetExtensionsStringARB");
		wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC) wglGetProcAddress("wglChoosePixelFormatARB");
		wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC) wglGetProcAddress("wglCreateContextAttribsARB");
	}

	HGLRC hglrc;

	if (wglGetExtensionsStringARB) 
	{
		const char * extension_string = wglGetExtensionsStringARB(window_data->GetDeviceContext());
		if (IsExtensionSupported(extension_string, "WGL_ARB_pixel_format") && 
			IsExtensionSupported(extension_string, "WGL_ARB_create_context"))
		{
			PIXELFORMATDESCRIPTOR dummy_pfd;
			SetPixelFormat(window_data->GetDeviceContext(), render_context->GetPimpl()->GetPixelFormatId(), &dummy_pfd);
			CreateAdvancedContext(window_data->GetWindowHandle(), window_data->GetDeviceContext(), major_version, minor_version, hglrc);
		}
		else
		{
			PIXELFORMATDESCRIPTOR dummy_pfd;
			SetPixelFormat(window_data->GetDeviceContext(), render_context->GetPimpl()->GetPixelFormatId(), &dummy_pfd);
			CreateBasicContext(window_data->GetWindowHandle(), window_data->GetDeviceContext(), hglrc);
		}
	}
	else
	{
		PIXELFORMATDESCRIPTOR dummy_pfd;
		SetPixelFormat(window_data->GetDeviceContext(), render_context->GetPimpl()->GetPixelFormatId(), &dummy_pfd);
		CreateBasicContext(window_data->GetWindowHandle(), window_data->GetDeviceContext(), hglrc);
	}

	if (false_hwnd)
	{
		ReleaseDC(false_hwnd, false_hdc);
		DestroyWindow(false_hwnd);
		wglMakeCurrent(0, 0);
		wglDeleteContext(false_hglrc);
	}

	if (!hglrc)
	{
		std::cout << "Render Context Initialization Failed." << std::endl;
	}
	else
	{
		this->pixelformat_id = render_context->GetPimpl()->GetPixelFormatId();
		this->hgl_render_context = hglrc;
		if (!wglShareLists(render_context->GetPimpl()->GetHGLRC(), hglrc))
		{
			std::cout << "Failed To Share Data Between Contexts." << std::endl;
		}
	}
}

RenderContextPimpl::~RenderContextPimpl()
{
	wglDeleteContext(hgl_render_context);
}

void RenderContextPimpl::MakeFalseWindowAndContext(HWND & rtn_hwnd, HDC & rtn_hdc, int & rtn_pixelformat_id, HGLRC & rtn_hglrc)
{
	HINSTANCE hInstance = GetModuleHandle(NULL);

	//Create window from currently registered window class and get window handle.
	RECT rect = { 0, 0, 100, 100 };
	AdjustWindowRect(&rect, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX, FALSE);

	rtn_hwnd = CreateWindow( "WindowClass", 
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
	if (!rtn_hwnd)
	{
		std::cout << "Error creating dummy window." << std::endl;
		throw 1;
	}

	//Get Device Context.
	rtn_hdc = GetDC(rtn_hwnd);

	//Set Pixel Format
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags =
		PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL |
		PFD_GENERIC_ACCELERATED |
		PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;

	rtn_pixelformat_id = ChoosePixelFormat(rtn_hdc, &pfd);
	SetPixelFormat(rtn_hdc, rtn_pixelformat_id, &pfd);

	//Create context
	rtn_hglrc = wglCreateContext(rtn_hdc);

	if (!rtn_hglrc)
	{
		std::cout << "False Context Creation Failed." << std::endl;
		throw 1;
	}

	wglMakeCurrent(rtn_hdc, rtn_hglrc);
}

void RenderContextPimpl::CreateAdvancedContext(const HWND hwnd, const HDC hdc, const int major_version, const int minor_version, HGLRC & rtn_hglrc)
{
	//Create OpenGL Context
	if (major_version == 3 && minor_version > 1)
	{
		int context_attribs[] =
		{
			WGL_CONTEXT_MAJOR_VERSION_ARB, major_version,
			WGL_CONTEXT_MINOR_VERSION_ARB, minor_version,
			WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
			0
		};

		rtn_hglrc = wglCreateContextAttribsARB(hdc, 0, context_attribs);

		if (!rtn_hglrc)
		{
			std::cout << "Advanced Context Creation Failed." << std::endl;
			throw 1;
		}

		wglMakeCurrent(hdc, rtn_hglrc);
	}
	else
	{
		int context_attribs[] =
		{
			WGL_CONTEXT_MAJOR_VERSION_ARB, major_version,
			WGL_CONTEXT_MINOR_VERSION_ARB, minor_version,
			0
		};

		rtn_hglrc = wglCreateContextAttribsARB(hdc, 0, context_attribs);

		if (!rtn_hglrc)
		{
			std::cout << "Advanced Context Creation Failed." << std::endl;
			throw 1;
		}

		wglMakeCurrent(hdc, rtn_hglrc);
	}
}

void RenderContextPimpl::SetAdvancedPixelFormat(const HWND hwnd, const HDC hdc, const WindowPixelFormat * pixel_format, int & rtn_pixelformat_id)
{
	unsigned int format_count;
	PIXELFORMATDESCRIPTOR dummy_pfd;

	bool use_multisample = pixel_format->GetSampleBuffers() > 0 && pixel_format->GetSamples() > 1;
	bool pixelformat_valid;
	if (use_multisample)
	{
		const int attribute_list_full[] =
		{ 
			WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
			WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
			WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
			WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
			WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
			WGL_RED_BITS_ARB, pixel_format->GetRedBits(),
			WGL_GREEN_BITS_ARB, pixel_format->GetGreenBits(),
			WGL_BLUE_BITS_ARB, pixel_format->GetBlueBits(),
			WGL_ALPHA_BITS_ARB, pixel_format->GetAlphaBits(),
			WGL_DEPTH_BITS_ARB, pixel_format->GetDepthBits(),
			WGL_STENCIL_BITS_ARB, pixel_format->GetStencilBits(),
			WGL_SAMPLE_BUFFERS_ARB, pixel_format->GetSampleBuffers(),
			WGL_SAMPLES_ARB, pixel_format->GetSamples(),
			0
		};
		
		pixelformat_valid = wglChoosePixelFormatARB(hdc, attribute_list_full, NULL, 1, &rtn_pixelformat_id, &format_count);
	}
	else
	{
		const int attribute_list_full[] =
		{ 
			WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
			WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
			WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
			WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
			WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
			WGL_RED_BITS_ARB, pixel_format->GetRedBits(),
			WGL_GREEN_BITS_ARB, pixel_format->GetGreenBits(),
			WGL_BLUE_BITS_ARB, pixel_format->GetBlueBits(),
			WGL_ALPHA_BITS_ARB, pixel_format->GetAlphaBits(),
			WGL_DEPTH_BITS_ARB, pixel_format->GetDepthBits(),
			WGL_STENCIL_BITS_ARB, pixel_format->GetStencilBits(),
			0
		};
		
		pixelformat_valid = wglChoosePixelFormatARB(hdc, attribute_list_full, NULL, 1, &rtn_pixelformat_id, &format_count);
	}

	if (!pixelformat_valid) 
	{
		std::cout << "Pixel Format Selection Failed. Using Default Pixel Format" << std::endl;
		const int attrib_list_default[] =
		{
			WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
			WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
			WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
			WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
			WGL_COLOR_BITS_ARB, 32,
			WGL_DEPTH_BITS_ARB, 24,
			WGL_STENCIL_BITS_ARB, 8,
			0
		};
		pixelformat_valid = wglChoosePixelFormatARB(hdc, attrib_list_default, NULL, 1, &rtn_pixelformat_id, &format_count);	
	}

	if (!pixelformat_valid)
	{
		std::cout << "Pixel Format Selection Failed. Advanced Context Creation Failed." << std::endl;
		throw 1;
	}
	
	SetPixelFormat(hdc, rtn_pixelformat_id, &dummy_pfd);
}
void RenderContextPimpl::CreateBasicContext(const HWND hwnd, const HDC hdc, HGLRC & rtn_hglrc)
{
	//Create context
	rtn_hglrc = wglCreateContext(hdc);

	if (!rtn_hglrc)
	{
		std::cout << "Basic Context Creation Failed." << std::endl;
	}

	wglMakeCurrent(hdc, rtn_hglrc);
}

void RenderContextPimpl::SetBasicPixelFormat(const HWND hwnd, const HDC hdc, const WindowPixelFormat * pixel_format, int & rtn_pixelformat_id)
{
	//Set Pixel Format
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags =
		PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL |
		PFD_GENERIC_ACCELERATED |
		PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = pixel_format->GetTotalColorBits();
	pfd.cDepthBits = pixel_format->GetDepthBits();
	pfd.cStencilBits = pixel_format->GetStencilBits();

	rtn_pixelformat_id = ChoosePixelFormat(hdc, &pfd);
	SetPixelFormat(hdc, rtn_pixelformat_id, &pfd);
}