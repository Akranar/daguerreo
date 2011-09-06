#include "../RenderContext.h"
#include "RenderContextPimpl_LINUX.h"
//#include "WindowData_LINUX.h"

RenderContext::RenderContext(WindowData * window_data, const WindowPixelFormat * pixel_format)
:
data(new RenderContextPimpl(window_data, pixel_format))
{

}

RenderContext::RenderContext(WindowData * window_data, const RenderContext * render_context)
:
data(new RenderContextPimpl(window_data, render_context))
{

}

RenderContext::~RenderContext()
{
	delete data;
}