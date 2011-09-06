#ifndef _RENDER_CONTEXT_PIMPL_MACOSX_H_
#define _RENDER_CONTEXT_PIMPL_MACOSX_H_

#import <Cocoa/Cocoa.h>

class WindowData;
class WindowPixelFormat;
class RenderContext;

class RenderContextPimpl
{
    NSOpenGLContext * ns_opengl_context;
    NSOpenGLPixelFormat * ns_opengl_pixelformat;
    void SetAdvancedPixelFormat(const WindowPixelFormat * pixel_format);
    void SetAdvancedMultisamplingPixelFormat(const WindowPixelFormat * pixel_format);
    void SetBasicPixelFormat(const WindowPixelFormat * pixel_format);
public:
	RenderContextPimpl(WindowData * window_data, const WindowPixelFormat * pixel_format);
	RenderContextPimpl(WindowData * window_data, const RenderContext * render_context);
	~RenderContextPimpl();

    inline NSOpenGLContext * GetContextObject() const;
    inline NSOpenGLPixelFormat * GetPixelFormatObject() const;
};

inline NSOpenGLContext * RenderContextPimpl::GetContextObject() const
{
	return ns_opengl_context;
}

inline NSOpenGLPixelFormat * RenderContextPimpl::GetPixelFormatObject() const
{
    return ns_opengl_pixelformat;
}



#endif

