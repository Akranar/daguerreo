#include "WindowEvents.h"
#include "RenderContext.h"

//Render Context Creation
RenderContext * WindowEvents::NewRenderContext(const WindowPixelFormat * pixel_format)
{
	if (!initialized) return 0;
	if (render_context)
	{
		std::cout << "Context already exists for this window." << std::endl;
		throw 1;
	}
	render_context = new RenderContext(data, pixel_format);
	return render_context;
}

RenderContext * WindowEvents::NewRenderContextShared(RenderContext * share_render_context)
{
	if (!initialized) return 0;
	if (render_context)
	{
		std::cout << "Context already exists for this window." << std::endl;
		throw 1;
	}
	render_context = new RenderContext(data, share_render_context);
	return render_context;
}
