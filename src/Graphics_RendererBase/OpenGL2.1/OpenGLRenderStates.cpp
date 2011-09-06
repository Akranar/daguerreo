#include "OpenGLRenderStates.h"
#include "OpenGLCore.h"
#include <iostream>

//bool blend_enabled, GLenum alpha_func, GLclampf reference, GLenum src_factor, GLenum dest_factor
//bool cull_enabled, GLenum cull_mode
//bool depthtest_enabled, GLboolean write_access, GLenum depth_func
//bool offset_fill, bool offset_line, bool offset_point, GLfloat factor, GLfloat units
//bool stenciltest_enabled, GLenum test_function, GLint ref, GLuint mask, GLuint stencil_mask, GLenum stencil_fail_op, GLenum depth_fail_op, GLenum depth_pass_op
//GLenum face, GLenum rasterize_mode
void OpenGLRenderStates::SetDefaultStates()
{
	//Alpha
	glDisable(GL_BLEND);
	glAlphaFunc(GL_ALWAYS, 0);
	glBlendFunc(GL_ONE, GL_ZERO);

	//Cull
	glDisable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//Depth
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);

	//PolygonOffset
	glDisable(GL_POLYGON_OFFSET_FILL);
    glDisable(GL_POLYGON_OFFSET_LINE);
    glDisable(GL_POLYGON_OFFSET_POINT);
    glPolygonOffset(0, 0);

	//Stencil
	glDisable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 0, -1);
	glStencilMask(-1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	//Wire
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//-------------------------------------------
	//Front Face
	glFrontFace(GL_CCW);

	//Color
	glColor3f(1.0f, 1.0f, 1.0f);

	//Clear Color
	glClearColor(0.4f, 0.4f, 1.0f, 1.0f);
}

void OpenGLRenderStates::SetAlphaState(bool blend_enabled, GLenum alpha_func, GLclampf reference, GLenum src_factor, GLenum dest_factor)
{
	blend_enabled ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
	glAlphaFunc(alpha_func, reference);
	glBlendFunc(src_factor, dest_factor);
}
void OpenGLRenderStates::SetCullState(bool cull_enabled, GLenum cull_mode)
{
	cull_enabled ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
	glCullFace(cull_mode);
}
void OpenGLRenderStates::SetDepthState(bool depthtest_enabled, GLboolean write_access, GLenum depth_func)
{
	depthtest_enabled ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
	glDepthMask(write_access);
	glDepthFunc(depth_func);
}
void OpenGLRenderStates::SetPolygonOffsetState(bool offset_fill, bool offset_line, bool offset_point, GLfloat factor, GLfloat units)
{
	offset_fill ? glEnable(GL_POLYGON_OFFSET_FILL) : glDisable(GL_POLYGON_OFFSET_FILL);
    offset_line ? glEnable(GL_POLYGON_OFFSET_LINE) : glDisable(GL_POLYGON_OFFSET_LINE);
    offset_point ? glEnable(GL_POLYGON_OFFSET_POINT) : glDisable(GL_POLYGON_OFFSET_POINT);
    glPolygonOffset(factor, units);
}
void OpenGLRenderStates::SetStencilState(bool stenciltest_enabled, GLenum test_function, GLint ref, GLuint mask, 
	GLuint stencil_mask, 
	GLenum stencil_fail_op, GLenum depth_fail_op, GLenum depth_pass_op)
{
	stenciltest_enabled ? glEnable(GL_STENCIL_TEST) : glDisable(GL_STENCIL_TEST);
	glStencilFunc(test_function, ref, mask);
	glStencilMask(stencil_mask);
	glStencilOp(stencil_fail_op, depth_fail_op, depth_pass_op);
}
void OpenGLRenderStates::SetWireState(GLenum face, GLenum rasterize_mode)
{
	glPolygonMode(face, rasterize_mode);
}

