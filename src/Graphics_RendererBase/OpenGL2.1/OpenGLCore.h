#ifndef _OPENGL_CORE_H_
#define _OPENGL_CORE_H_

class OpenGLCore
{
public:
	static void PrintAllErrors();
	static void SetViewport(unsigned int width, unsigned int height);
	static void Initialize();
};

#endif