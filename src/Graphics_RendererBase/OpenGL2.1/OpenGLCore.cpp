#include "OpenGLCore.h"
#include <iostream>
#include "ExLib_API_OpenGL.h"

void OpenGLCore::Initialize()
{
	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW Initialization Failed." << std::endl;
		throw 1;
	}

	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
}

void OpenGLCore::PrintAllErrors()
{
	GLenum error;
	error = glGetError();
	while (error != GL_NO_ERROR)
	{
		switch (error)
		{
			case GL_INVALID_ENUM:
				std::cout << "GL_INVALID_ENUM" << std::endl;
				break;
			case GL_INVALID_VALUE:
				std::cout << "GL_INVALID_VALUE" << std::endl;
				break;
			case GL_INVALID_OPERATION:
				std::cout << "GL_INVALID_OPERATION" << std::endl;
				break;
			case GL_STACK_OVERFLOW:
				std::cout << "GL_STACK_OVERFLOW" << std::endl;
				break;
			case GL_STACK_UNDERFLOW:
				std::cout << "GL_STACK_UNDERFLOW" << std::endl;
				break;
			case GL_OUT_OF_MEMORY:
				std::cout << "GL_OUT_OF_MEMORY" << std::endl;
				break;
			case GL_TABLE_TOO_LARGE:
				std::cout << "GL_TABLE_TOO_LARGE" << std::endl;
				break;
			default:
				break;
		}
		error = glGetError();
	}


}

void OpenGLCore::SetViewport(unsigned int width, unsigned int height)
{
	glViewport(0, 0, width, height);
}