#ifndef _EXAMPLE_MESH_QUAD_H_
#define _EXAMPLE_MESH_QUAD_H_

#include "RendererBase.h"
#include "ExLib_ProceduralMesh.h"
#include "IncludeAll_Interface.h"
#include "IncludeAll_Desc.h"


class Example_MeshQuad
{
public:
	VertexBuffer vertex_buffer;
	IndexBuffer index_buffer;

	unsigned int byte_stride;
	unsigned int index_count;

	RendererBase * renderer;

	DescVertexFormat vertex_format;
	
	void Init(RendererBase * _renderer)
	{
		renderer = _renderer;

		ProceduralQuad quad;
		Eigen::Vector3f corners[4];
		float radius = 10;
		corners[0] = Eigen::Vector3f(-radius, -radius, -2); 
		corners[1] = Eigen::Vector3f(radius, -radius, -2); 
		corners[2] = Eigen::Vector3f(radius, radius, -2); 
		corners[3] = Eigen::Vector3f(-radius, radius, -2);
		quad.SetOptions(false); //Currently unused.
		quad.SetCorners(corners[0],corners[1], corners[2], corners[3]);
		quad.SetSections(16, 16);

		unsigned int vertex_count = quad.GetVertexCount();
		index_count = quad.GetTerrainStripIndexCount();
		byte_stride = sizeof(float) * 8;

		DescVertexBuffer vb_desc;
		vb_desc.ByteCount = byte_stride * vertex_count;
		vb_desc.ByteStride = byte_stride;
		
		DescIndexBuffer ib_desc;
		ib_desc.ByteCount = index_count * sizeof(unsigned int);
		ib_desc.PrimitiveType = PRIMITIVE_TRIANGLE_STRIP;
		
		unsigned char * initial_vertex_data = new unsigned char [vb_desc.ByteCount];
		unsigned char * initial_index_data = new unsigned char [ib_desc.ByteCount];
		{
			quad.GetPositions(initial_vertex_data, byte_stride);
			quad.GetNormals(initial_vertex_data + sizeof(float) * 3, byte_stride);
			quad.GetTexcoord2D(initial_vertex_data + sizeof(float) * 6, byte_stride);
			quad.GetTerrainStripIndices(initial_index_data);

			vb_desc.Usage = BUFFER_USAGE_STATIC;
			vb_desc.InitialData = initial_vertex_data;

			ib_desc.Usage = BUFFER_USAGE_STATIC;
			ib_desc.InitialData = initial_index_data;

			renderer->CreateResource(vb_desc, &vertex_buffer);
			renderer->CreateResource(ib_desc, &index_buffer);

		}
		delete initial_vertex_data;
		delete initial_index_data;
		
		vertex_format.format_entries.Set(3);
		vertex_format[0].SetName("POSITION");
		vertex_format[0].SetSemanticIndex(0);
		vertex_format[0].SetType(DT_FLOAT);
		vertex_format[0].SetStride(byte_stride);
		vertex_format[0].SetChannels(3);
		vertex_format[0].SetOffset(0);
		vertex_format[0].SetStreamIndex(0);

		vertex_format[1].SetName("NORMAL");
		vertex_format[1].SetSemanticIndex(0);
		vertex_format[1].SetType(DT_FLOAT);
		vertex_format[1].SetStride(byte_stride);
		vertex_format[1].SetChannels(3);
		vertex_format[1].SetOffset(sizeof(float) * 3);
		vertex_format[1].SetStreamIndex(0);


		vertex_format[2].SetName("TEXCOORD");
		vertex_format[2].SetSemanticIndex(0);
		vertex_format[2].SetType(DT_FLOAT);
		vertex_format[2].SetStride(byte_stride);
		vertex_format[2].SetChannels(2);
		vertex_format[2].SetOffset(sizeof(float) * 6);
		vertex_format[2].SetStreamIndex(0);
		vertex_buffer.format_hash = renderer->GetVertexFormatHash(vertex_format);
	}

	void InitWithShader(ShaderProgram * shader_program)
	{
		renderer->CreateInputLayout(vertex_buffer.format_hash, shader_program);
	}

	void Deinit(RendererBase & renderer)
	{
		renderer.Delete(&vertex_buffer);
		renderer.Delete(&index_buffer);
	}
	void Update(double time)
	{
		//torus.SetOptions(32, 16, 0.75f, 1.0f, true);
		//torus.SetOptions(rings, sides, fabs((float) cos(time * 1.0)), max_radius, true, false);
	}
	void PreRender()
	{
		renderer->BindVertexBuffer(&vertex_buffer);
		renderer->BindIndexBuffer(&index_buffer);
	}
	void PreRenderWithShader(ShaderProgram * shader_program)
	{
		renderer->SetInputLayout(vertex_buffer.format_hash, shader_program->signature_hash);
	}
	void PostRender(RendererBase & renderer)
	{
	}
};

#endif