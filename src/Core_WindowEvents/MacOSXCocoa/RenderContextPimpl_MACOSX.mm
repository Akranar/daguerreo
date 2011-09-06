#include "RenderContextPimpl_MACOSX.h"
#include "Cocoa_WindowData.h"
#include "../WindowPixelFormat.h"
#include "../RenderContext.h"
#include <iostream>


RenderContextPimpl::RenderContextPimpl(WindowData * window_data, const WindowPixelFormat * pixel_format)
{
	bool use_multisample = pixel_format->GetSampleBuffers() > 0 && pixel_format->GetSamples() > 1;
	
	if (use_multisample)
	{
		SetAdvancedMultisamplingPixelFormat(pixel_format);
		
		if(!ns_opengl_pixelformat)
		{
			std::cout << "Multisampling Pixel Format Creation Failed." << std::endl;
			SetAdvancedPixelFormat(pixel_format);
		}
	}
	else 
	{
		SetAdvancedPixelFormat(pixel_format);
	}
	
	if (!ns_opengl_pixelformat)
	{
		std::cout << "Advanced Pixel Format Creation Failed." << std::endl;
		SetBasicPixelFormat(pixel_format);
	}

	
	if (!ns_opengl_pixelformat)
	{
		std::cout << "Basic Pixel Format Creation Failed" << std::endl;
		throw 1;
	}
	
	ns_opengl_context = [[NSOpenGLContext alloc] initWithFormat:ns_opengl_pixelformat shareContext:nil];
	[ns_opengl_context makeCurrentContext];
	
	if (!ns_opengl_context)
	{
		std::cout << "Context Creation Failed." << std::endl;
		throw 1;
	}
}

RenderContextPimpl::RenderContextPimpl(WindowData * window_data, const RenderContext * render_context)
{
	ns_opengl_pixelformat = [[NSOpenGLPixelFormat alloc] initWithCGLPixelFormatObj:[render_context->GetPimpl()->GetPixelFormatObject() CGLPixelFormatObj]];
	ns_opengl_context = [[NSOpenGLContext alloc] initWithFormat:ns_opengl_pixelformat shareContext:render_context->GetPimpl()->GetContextObject()];
	[ns_opengl_context makeCurrentContext];
	
	if (!ns_opengl_context)
	{
		std::cout << "Context Creation Failed." << std::endl;
		throw 1;
	}
}

RenderContextPimpl::~RenderContextPimpl()
{
	[ns_opengl_context release];
	[ns_opengl_pixelformat release];

}

void RenderContextPimpl::SetAdvancedMultisamplingPixelFormat(const WindowPixelFormat * pixel_format)
{
	NSOpenGLPixelFormatAttribute attributes_multisampling[] =
	{
		NSOpenGLPFAWindow,
		NSOpenGLPFAAccelerated,
		NSOpenGLPFADoubleBuffer,
		NSOpenGLPFANoRecovery,
		NSOpenGLPFAMinimumPolicy,
		NSOpenGLPFAScreenMask, CGDisplayIDToOpenGLDisplayMask(CGMainDisplayID()),
		NSOpenGLPFAColorSize, (NSOpenGLPixelFormatAttribute) pixel_format->GetTotalColorBits(),
		NSOpenGLPFAAlphaSize, (NSOpenGLPixelFormatAttribute) pixel_format->GetAlphaBits(),
		NSOpenGLPFADepthSize, (NSOpenGLPixelFormatAttribute) pixel_format->GetDepthBits(),
		NSOpenGLPFAStencilSize, (NSOpenGLPixelFormatAttribute) pixel_format->GetStencilBits(),
		NSOpenGLPFAMultisample,
		NSOpenGLPFASampleBuffers, pixel_format->GetSampleBuffers(),
		NSOpenGLPFASamples, pixel_format->GetSamples(),
		NSOpenGLPFASampleAlpha,
		(NSOpenGLPixelFormatAttribute)nil
	};
	
	ns_opengl_pixelformat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attributes_multisampling];
}

void RenderContextPimpl::SetAdvancedPixelFormat(const WindowPixelFormat * pixel_format)
{
	NSOpenGLPixelFormatAttribute attributes_advanced[] =
	{
		NSOpenGLPFAWindow,
		NSOpenGLPFAAccelerated,
		NSOpenGLPFADoubleBuffer,
		NSOpenGLPFANoRecovery,
		NSOpenGLPFAMinimumPolicy,
		NSOpenGLPFAScreenMask, CGDisplayIDToOpenGLDisplayMask(CGMainDisplayID()),
		NSOpenGLPFAColorSize, (NSOpenGLPixelFormatAttribute) pixel_format->GetTotalColorBits(),
		NSOpenGLPFAAlphaSize, (NSOpenGLPixelFormatAttribute) pixel_format->GetAlphaBits(),
		NSOpenGLPFADepthSize, (NSOpenGLPixelFormatAttribute) pixel_format->GetDepthBits(),
		NSOpenGLPFAStencilSize, (NSOpenGLPixelFormatAttribute) pixel_format->GetStencilBits(),
		(NSOpenGLPixelFormatAttribute)nil
	};
	
	ns_opengl_pixelformat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attributes_advanced];
}
void RenderContextPimpl::SetBasicPixelFormat(const WindowPixelFormat * pixel_format)
{
	NSOpenGLPixelFormatAttribute attributes_basic [] =
	{
		NSOpenGLPFAWindow,
		NSOpenGLPFADoubleBuffer,
		NSOpenGLPFAColorSize,
		(NSOpenGLPixelFormatAttribute)32,
		NSOpenGLPFADepthSize,
		(NSOpenGLPixelFormatAttribute)16,
		NSOpenGLPFAStencilSize,
		(NSOpenGLPixelFormatAttribute)8,
		NSOpenGLPFAScreenMask,
		CGDisplayIDToOpenGLDisplayMask(CGMainDisplayID()),
		(NSOpenGLPixelFormatAttribute)nil
        
	};
	ns_opengl_pixelformat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attributes_basic];
	
}

