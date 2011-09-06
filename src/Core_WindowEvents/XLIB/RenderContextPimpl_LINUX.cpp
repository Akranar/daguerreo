#include "RenderContextPimpl_LINUX.h"
#include "WindowData_LINUX.h"
#include "../WindowPixelFormat.h"
#include "../RenderContext.h"
#include <iostream>

#include <GL/glext.h>
#include <GL/glxext.h>

#include <cstring>

glXCreateContextAttribsARBProc RenderContextPimpl::glXCreateContextAttribsARB = 0;

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

    bool use_multisampling = pixel_format->GetSampleBuffers() > 0 && pixel_format->GetSamples() > 1;

    bool pixel_format_set = false;
    if (use_multisampling)
    {
        pixel_format_set = SetAdvancedMultisamplingPixelFormat(window_data->GetDisplay(), pixel_format, this->glx_fbconfig);

        if (!pixel_format_set)
        {
            std::cout << "Multisampling Pixel Format Selection Failed" << std::endl;
            pixel_format_set = SetAdvancedPixelFormat(window_data->GetDisplay(), pixel_format, this->glx_fbconfig);
        }
    }
    else
    {
        pixel_format_set = SetAdvancedPixelFormat(window_data->GetDisplay(), pixel_format, this->glx_fbconfig);
    }

    if (!pixel_format_set)
    {
        std::cout << "Advanced Pixel Format Selection Failed" << std::endl;
        pixel_format_set = SetBasicPixelFormat(window_data->GetDisplay(), pixel_format, this->glx_fbconfig);
    }

    if (!pixel_format_set)
    {
        std::cout << "Pixel Format Selection Failed" << std::endl;
        throw 1;
    }

    window_data->RecreateWindow(this->glx_fbconfig);

    if (!glXCreateContextAttribsARB)
    {
        glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc) glXGetProcAddressARB( (const GLubyte *) "glXCreateContextAttribsARB");
    }

    const char *glx_extentions = glXQueryExtensionsString(window_data->GetDisplay(), DefaultScreen(window_data->GetDisplay()));

    int context_attribs[] =
    {
        GLX_CONTEXT_MAJOR_VERSION_ARB,
        3,
        GLX_CONTEXT_MINOR_VERSION_ARB,
        0,
        None
    };

    if (!IsExtensionSupported(glx_extentions, "GLX_ARB_create_context") || !glXCreateContextAttribsARB)
    {
        std::cout << "GLX_ARB_create_context Extension Not Available.  Creating Basic OpenGL Context.";
        glx_context = glXCreateNewContext(window_data->GetDisplay(), glx_fbconfig, GLX_RGBA_TYPE, 0, true);
    }
    else
    {
        glx_context = glXCreateContextAttribsARB(window_data->GetDisplay(), glx_fbconfig, 0, true, context_attribs);
    }

    if (!glx_context)
    {
        std::cout << "OpenGL Context Creation Failed." << std::endl;
        throw 1;
    }


    glXMakeCurrent(window_data->GetDisplay(), window_data->GetWindow(), glx_context);
}

RenderContextPimpl::RenderContextPimpl(WindowData * window_data, const RenderContext * render_context)
{
    this->glx_fbconfig = render_context->GetPimpl()->GetGLXFBConfig();

    window_data->RecreateWindow(this->glx_fbconfig);

    const char *glx_extentions = glXQueryExtensionsString(window_data->GetDisplay(), DefaultScreen(window_data->GetDisplay()));

    int context_attribs[] =
    {
        GLX_CONTEXT_MAJOR_VERSION_ARB,
        3,
        GLX_CONTEXT_MINOR_VERSION_ARB,
        0,
        None
    };

    if (!IsExtensionSupported(glx_extentions, "GLX_ARB_create_context") || !glXCreateContextAttribsARB)
    {
        std::cout << "GLX_ARB_create_context Extension Not Available.  Creating Basic OpenGL Context.";
        glx_context = glXCreateNewContext(window_data->GetDisplay(), glx_fbconfig, GLX_RGBA_TYPE, render_context->GetPimpl()->GetGLXContext(), true);
    }
    else
    {
        glx_context = glXCreateContextAttribsARB(window_data->GetDisplay(), glx_fbconfig, render_context->GetPimpl()->GetGLXContext(), true, context_attribs);
    }

    if (!glx_context)
    {
        std::cout << "OpenGL Context Creation Failed." << std::endl;
        throw 1;
    }


    glXMakeCurrent(window_data->GetDisplay(), window_data->GetWindow(), glx_context);
}

RenderContextPimpl::~RenderContextPimpl()
{

}

bool RenderContextPimpl::SetAdvancedMultisamplingPixelFormat(Display * display, const WindowPixelFormat * pixel_format, GLXFBConfig & rtn_fbconfig)
{
    int attributes_multisampling[] =
	{
        GLX_X_RENDERABLE, true,
        GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
        GLX_RENDER_TYPE, GLX_RGBA_BIT,
        GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
        GLX_DOUBLEBUFFER, true,
        GLX_RED_SIZE, pixel_format->GetRedBits(),
        GLX_GREEN_SIZE, pixel_format->GetGreenBits(),
        GLX_BLUE_SIZE, pixel_format->GetBlueBits(),
        GLX_ALPHA_SIZE, pixel_format->GetAlphaBits(),
        GLX_DEPTH_SIZE, pixel_format->GetDepthBits(),
        GLX_STENCIL_SIZE, pixel_format->GetStencilBits(),
        GLX_SAMPLE_BUFFERS_ARB, pixel_format->GetSampleBuffers(),
        GLX_SAMPLES_ARB, pixel_format->GetSamples(),
        None
	};

    int elements_count;
    GLXFBConfig *fbc = glXChooseFBConfig(display, DefaultScreen(display), attributes_multisampling, &elements_count);

    if (elements_count == 0) return false;
    rtn_fbconfig = fbc[0];
    return true;
}

bool RenderContextPimpl::SetAdvancedPixelFormat(Display * display, const WindowPixelFormat * pixel_format, GLXFBConfig & rtn_fbconfig)
{

    int attributes_advanced[] =
	{
        GLX_X_RENDERABLE, true,
        GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
        GLX_RENDER_TYPE, GLX_RGBA_BIT,
        GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
        GLX_DOUBLEBUFFER, true,
        GLX_RED_SIZE, pixel_format->GetRedBits(),
        GLX_GREEN_SIZE, pixel_format->GetGreenBits(),
        GLX_BLUE_SIZE, pixel_format->GetBlueBits(),
        GLX_ALPHA_SIZE, pixel_format->GetAlphaBits(),
        GLX_DEPTH_SIZE, pixel_format->GetDepthBits(),
        GLX_STENCIL_SIZE, pixel_format->GetStencilBits(),
        None
	};

    int elements_count;
	GLXFBConfig *fbc = glXChooseFBConfig(display, DefaultScreen(display), attributes_advanced, &elements_count);

    if (elements_count == 0) return false;
    rtn_fbconfig = fbc[0];
    return true;
}

bool RenderContextPimpl::SetBasicPixelFormat(Display * display, const WindowPixelFormat * pixel_format, GLXFBConfig & rtn_fbconfig)
{
    int attributes_basic[] =
	{
        GLX_X_RENDERABLE, true,
		GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
		GLX_RENDER_TYPE, GLX_RGBA_BIT,
		GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
		GLX_RED_SIZE, 8,
		GLX_GREEN_SIZE, 8,
		GLX_BLUE_SIZE, 8,
		GLX_ALPHA_SIZE, 8,
		GLX_DEPTH_SIZE, 24,
		GLX_STENCIL_SIZE, 8,
		GLX_DOUBLEBUFFER, true,
        None
	};

    int elements_count;
	GLXFBConfig *fbc = glXChooseFBConfig(display, DefaultScreen(display), attributes_basic, &elements_count);

    if (elements_count == 0) return false;
    rtn_fbconfig = fbc[0];
    return true;

}
