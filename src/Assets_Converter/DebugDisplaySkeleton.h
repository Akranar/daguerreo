#ifndef _DEBUG_DISPLAY_SKELETON_H_
#define _DEBUG_DISPLAY_SKELETON_H_

#include <vector>

class FloatDataAccessor;
class PolyLines;
class VertexFormat;
class VertexBuffer;
class IndexBuffer;
class VertexShader;
class FragmentShader;
class VisualEffectClass;
class VisualEffectInstance;
class RendererBase;

class SkinnedMesh;

#include "ExLib_RendererBase.h"
#include "ExLib_Projector.h"

class DebugDisplaySkeleton
{
private:
	class DebugDisplaySkeletonInner
	{
	public:
		SkinnedMesh * skinned_mesh;
		VertexBuffer skeleton_display_vertex_buffer;
		IndexBuffer skeleton_display_index_buffer;

		~DebugDisplaySkeletonInner();
	};

	std::vector<DebugDisplaySkeletonInner> skeleton_displays;
	ShaderProgram shader_program;
	UniformBuffer mvp;
	unsigned int mvp_index;
	bool effect_instance_initialized;
	RendererBase * renderer;
	Projector * camera;
	DepthStencilState depth_stencil_state;
public:
	DebugDisplaySkeleton();

	~DebugDisplaySkeleton();
	void Init(RendererBase * _renderer, Projector * _camera);
	void Update(unsigned int index);
	void PreDraw(unsigned int index);
	void InitWithSkinnedMesh(SkinnedMesh * in_skeleton);
	void Deinit();
};

#endif