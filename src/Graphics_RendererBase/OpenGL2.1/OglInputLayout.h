#ifndef _OGL_INPUT_LAYOUT_H_
#define _OGL_INPUT_LAYOUT_H_

#include "ExLib_API_OpenGL.h"
#include <Core_FixedArray.h>

struct OglInputLayoutEntry
{
	unsigned char channels;
	GLuint type;
	unsigned int stride;
	unsigned int offset;
};

class OglInputLayout
{
public:
	FixedArray<OglInputLayoutEntry> attribs;

	void Begin();
	void End();
};

#endif