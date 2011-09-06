#ifndef _RENDER_CONTEXT_PIMPL_LINUX_H_
#define _RENDER_CONTEXT_PIMPL_LINUX_H_

#include <GL/glx.h>

class WindowData;
class WindowPixelFormat;
class RenderContext;

typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);

class RenderContextPimpl
{
	GLXContext glx_context;
	GLXFBConfig glx_fbconfig;
	static bool SetAdvancedMultisamplingPixelFormat(Display * display, const WindowPixelFormat * pixel_format, GLXFBConfig & rtn_fbconfig);
	static bool SetAdvancedPixelFormat(Display * display, const WindowPixelFormat * pixel_format, GLXFBConfig & rtn_fbconfig);
	static bool SetBasicPixelFormat(Display * display, const WindowPixelFormat * pixel_format, GLXFBConfig & rtn_fbconfig);

	static bool IsExtensionSupported(const char * extension_list, const char * extension);

    static glXCreateContextAttribsARBProc glXCreateContextAttribsARB;
public:
	RenderContextPimpl(WindowData * window_data, const WindowPixelFormat * pixel_format);
	RenderContextPimpl(WindowData * window_data, const RenderContext * render_context);
	~RenderContextPimpl();

	inline GLXContext GetGLXContext();
	inline GLXFBConfig GetGLXFBConfig();
};

inline GLXContext RenderContextPimpl::GetGLXContext()
{
    return glx_context;
}
inline GLXFBConfig RenderContextPimpl::GetGLXFBConfig()
{
    return glx_fbconfig;
}

#endif

