#ifndef _EXAMPLE_ASSIMP_MESH_H_
#define _EXAMPLE_ASSIMP_MESH_H_

#include "ExLib_RendererBase.h"
#include "RendererBaseMesh.h"
#include <Core_FixedArray.h>

class Example_AssimpMesh
{
public:
	FixedArray<VertexBuffer> vertex_buffers;
	FixedArray<IndexBuffer> index_buffers;
	RendererBase * renderer;
	void Init(RendererBase * _renderer, AssimpSceneData * _assimp_scene)
	{
		renderer = _renderer;
		
		vertex_buffers.Set(_assimp_scene->mesh_descs.GetSize());
		index_buffers.Set(_assimp_scene->mesh_descs.GetSize());
		for (unsigned int i = 0; i < _assimp_scene->mesh_descs.GetSize(); ++i)
		{
			renderer->CreateResource(_assimp_scene->mesh_descs[i].GetVertexDesc(), &vertex_buffers[i]);
			renderer->CreateResource(_assimp_scene->mesh_descs[i].GetIndexDesc(), &index_buffers[i]);
			vertex_buffers[i].format_hash = renderer->GetVertexFormatHash(_assimp_scene->mesh_descs[i].GetFormatDesc());
		}
	}

	void InitWithShader(ShaderProgram * shader_program, AssimpSceneData * _assimp_scene)
	{
		for (unsigned int i = 0; i < vertex_buffers.GetSize(); ++i)
		{
			renderer->CreateInputLayout(vertex_buffers[i].format_hash, shader_program);
		}
	}

	void Deinit(RendererBase & renderer)
	{
		for (unsigned int i = 0; i < vertex_buffers.GetSize(); ++i)
		{
			renderer.Delete(&vertex_buffers[i]);
			renderer.Delete(&index_buffers[i]);
		}
	}
	void Update(double time)
	{
		//torus.SetOptions(32, 16, 0.75f, 1.0f, true);
		//torus.SetOptions(rings, sides, fabs((float) cos(time * 1.0)), max_radius, true, false);
	}
	void PreRender(unsigned int i)
	{
		renderer->BindVertexBuffer(&vertex_buffers[i]);
		renderer->BindIndexBuffer(&index_buffers[i]);
	}
	void PreRenderWithShader(unsigned int i, ShaderProgram * shader_program)
	{
		renderer->SetInputLayout(vertex_buffers[i].format_hash, shader_program->signature_hash);
	}
	void PostRender(RendererBase & renderer)
	{
	}

	unsigned int GetMeshCount() const
	{
		return vertex_buffers.GetSize();
	}
};

#endif