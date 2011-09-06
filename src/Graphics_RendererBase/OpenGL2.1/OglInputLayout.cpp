#include "OglInputLayout.h"


void OglInputLayout::Begin()
{
	for (unsigned int i = 0; i < attribs.GetSize(); ++i)
	{
		glEnableVertexAttribArray(i);
		OglInputLayoutEntry & temp = attribs[i];
		//glVertexAttribPointer(i, attribs[i].channels, attribs[i].type, GL_FALSE, attribs[i].stride, (const GLvoid*) attribs[i].offset);
		glVertexAttribPointer(i, attribs[i].channels, GL_FLOAT, GL_FALSE, attribs[i].stride, (const GLvoid*) attribs[i].offset);
		//FIX TYPE
	}
}

void OglInputLayout::End()
{
	for (unsigned int i = 0; i < attribs.GetSize(); ++i)
	{
		glDisableVertexAttribArray(i);
	}
}