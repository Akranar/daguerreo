#ifndef _RENDER_CONTEXT_H_
#define _RENDER_CONTEXT_H_

class RenderContextPimpl;

class WindowData;
class WindowPixelFormat;

class RenderContext
{
	RenderContextPimpl * data;
public:
	RenderContext(WindowData * window_data, const WindowPixelFormat * pixel_format);
	RenderContext(WindowData * window_data, const RenderContext * render_context);
	~RenderContext();
	inline RenderContextPimpl * GetPimpl() const;
};

inline RenderContextPimpl * RenderContext::GetPimpl() const
{
	return data;
}

#endif