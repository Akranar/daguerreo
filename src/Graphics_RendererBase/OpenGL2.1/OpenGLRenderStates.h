#ifndef _OPENGL_RENDER_STATES_H_
#define _OPENGL_RENDER_STATES_H_

#include "ExLib_API_OpenGL.h"

class OpenGLRenderStates
{
public:
	//*************************
	//	Render States
	//*************************
	static void SetDefaultStates();
	static void SetAlphaState(bool blend_enabled, GLenum alpha_func, GLclampf reference, GLenum src_factor, GLenum dest_factor);
	static void SetCullState(bool cull_enabled, GLenum cull_mode);
	static void SetDepthState(bool depthtest_enabled, GLboolean write_access, GLenum depth_func);
	static void SetPolygonOffsetState(bool offset_fill, bool offset_line, bool offset_point, GLfloat factor, GLfloat units);
	static void SetStencilState(bool stenciltest_enabled, GLenum test_function, GLint ref, GLuint mask, 
		GLuint stencil_mask, 
		GLenum stencil_fail_op, GLenum depth_fail_op, GLenum depth_pass_op);
	static void SetWireState(GLenum face, GLenum rasterize_mode);

	//*************************
	//	Sampler States
	//*************************
	//	http://www.opengl.org/sdk/docs/man/xhtml/glTexParameter.xml
	//	GL_TEXTURE_MIN_FILTER
	//		-> GL_NEAREST | GL_LINEAR | GL_NEAREST_MIPMAP_NEAREST | GL_LINEAR_MIPMAP_NEAREST | GL_NEAREST_MIPMAP_LINEAR | GL_LINEAR_MIPMAP_LINEAR
	//	GL_TEXTURE_MAG_FILTER
	//		-> GL_NEAREST | GL_LINEAR
	//	GL_TEXTURE_MIN_LOD
	//		-> floating-point
	//	GL_TEXTURE_MAX_LOD
	//		-> floating-point
	//	GL_TEXTURE_BASE_LEVEL
	//		-> integer
	//	GL_TEXTURE_MAX_LEVEL
	//		-> integer
	//	GL_TEXTURE_WRAP_S
	//		-> GL_CLAMP | GL_CLAMP_TO_BORDER | GL_CLAMP_TO_EDGE | GL_MIRRORED_REPEAT | GL_REPEAT
	//	GL_TEXTURE_WRAP_T
	//		-> GL_CLAMP | GL_CLAMP_TO_BORDER | GL_CLAMP_TO_EDGE | GL_MIRRORED_REPEAT | GL_REPEAT
	//	GL_TEXTURE_WRAP_R
	//		-> GL_CLAMP | GL_CLAMP_TO_BORDER | GL_CLAMP_TO_EDGE | GL_MIRRORED_REPEAT | GL_REPEAT
	//	GL_TEXTURE_BORDER_COLOR
	//		-> R, G, B, A color
	//	GL_TEXTURE_PRIORITY
	//		-> floating_point 0 to 1
	//	GL_TEXTURE_COMPARE_MODE
	//		-> GL_COMPARE_R_TO_TEXTURE | GL_NONE
	//	GL_TEXTURE_COMPARE_FUNC
	//		-> GL_LEQUAL | GL_GEQUAL | GL_LESS | GL_GREATER | GL_EQUAL | GL_NOTEQUAL | GL_ALWAYS | GL_NEVER
	//	GL_DEPTH_TEXTURE_MODE
	//		-> GL_LUMINANCE | GL_INTENSITY | GL_ALPHA
	//	GL_GENERATE_MIPMAP
	//		-> GL_FALSE | GL_TRUE
	//

	static void SetSampler1D(int tex_unit);
	static void SetSampler2D(int tex_unit);
	static void SetSampler3D(int tex_unit);
	static void SetSamplerCube(int tex_unit);
	static void SetSamplerRect(int tex_unit);

};

#endif