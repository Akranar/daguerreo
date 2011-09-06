#ifndef _RENDER_CONTEXT_PIMPL_WINDOWS_H_
#define _RENDER_CONTEXT_PIMPL_WINDOWS_H_

#include <windows.h>
#include <GL/gl.h>
#define GL_GLEXT_PROTOTYPES 1
#include <GL/glext.h>
#define WGL_WGLEXT_PROTOTYPES 1
#include "wglext.h" //downloaded from http://www.opengl.org/registry/api/wglext.h

class WindowData;
class WindowPixelFormat;
class RenderContext;

class RenderContextPimpl
{
	HGLRC hgl_render_context;
	int pixelformat_id;
	static PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB;
	static PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
	static PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;

	static bool RenderContextPimpl::IsExtensionSupported(const char * extension_list, const char * extension);
	static void MakeFalseWindowAndContext(HWND & rtn_hwnd, HDC & rtn_hdc, int & rtn_pixelformat_id, HGLRC & rtn_hglrc);
	static void CreateAdvancedContext(const HWND hwnd, const HDC hdc, const int major_version, const int minor_version, HGLRC & rtn_hglrc);

	static void SetAdvancedPixelFormat(const HWND hwnd, const HDC hdc, const WindowPixelFormat * pixel_format, 
										int & rtn_pixelformat_id);

	static void CreateBasicContext(const HWND hwnd, const HDC hdc, HGLRC & rtn_hglrc);

	static void SetBasicPixelFormat(const HWND hwnd, const HDC hdc, const WindowPixelFormat * pixel_format, int & rtn_pixelformat_id);
public:
	RenderContextPimpl(WindowData * window_data, const WindowPixelFormat * pixel_format);
	RenderContextPimpl(WindowData * window_data, const RenderContext * render_context);
	~RenderContextPimpl();
	inline HGLRC & GetHGLRC();
	inline int GetPixelFormatId();
};

inline HGLRC & RenderContextPimpl::GetHGLRC()
{
	return hgl_render_context;
}

inline int RenderContextPimpl::GetPixelFormatId()
{
	return pixelformat_id;
}

#endif

