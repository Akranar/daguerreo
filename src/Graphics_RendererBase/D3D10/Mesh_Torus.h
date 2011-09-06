#ifndef _MESH_TORUS_H_
#define _MESH_TORUS_H_

#include "../RendererBase.h"
#include "../IncludeAll_Interface.h"
#include "../IncludeAll_Desc.h"
#include "../ExLib_ProceduralMesh.h"



class Mesh_Torus
{
public:
	VertexBuffer vertex_buffer;
	IndexBuffer index_buffer;
	//void * layout;
	unsigned int byte_stride;
	unsigned int index_count;

	ProceduralTorus torus;

	bool static_data;
	unsigned int rings, sides;
	float max_radius;
	
	
	void Update(double time)
	{
		torus.SetOptions(rings, sides, fabs(cos(time * 1.0f)), max_radius, true, false);
	}
	void Mesh_Torus::Init(RendererBase & renderer, ShaderProgram * shader_program)
	{
		max_radius = 1.1f;
		rings = 32;
		sides = 16;
		torus.SetOptions(rings, sides, 0.5f, max_radius, true, false);

		unsigned int vertex_count = torus.GetVertexCount();
		index_count = torus.GetTriangleIndexCount();
		byte_stride = sizeof(float) * 8;

		DescVertexBuffer vb_desc;
		vb_desc.ByteCount = byte_stride * vertex_count;
		vb_desc.ByteStride = byte_stride;
		
		DescIndexBuffer ib_desc;
		ib_desc.ByteCount = index_count * sizeof(unsigned int);
		
		static_data = false;
		if (static_data)
		{
			unsigned char * initial_vertex_data = new unsigned char [vb_desc.ByteCount];
			unsigned char * initial_index_data = new unsigned char [ib_desc.ByteCount];

			torus.GetPositions(initial_vertex_data, byte_stride);
			torus.GetNormals(initial_vertex_data + sizeof(float) * 3, byte_stride);
			torus.GetTexcoord2D(initial_vertex_data + sizeof(float) * 6, byte_stride);

			torus.GetTriangles(initial_index_data);

			vb_desc.Usage = BUFFER_USAGE_STATIC;
			vb_desc.InitialData = initial_vertex_data;

			ib_desc.Usage = BUFFER_USAGE_STATIC;
			ib_desc.InitialData = initial_index_data;

			renderer.CreateResource(vb_desc, &vertex_buffer);
			renderer.CreateResource(ib_desc, &index_buffer);

			delete initial_vertex_data;
			delete initial_index_data;
		}
		else
		{
			vb_desc.Usage = BUFFER_USAGE_STREAM;
			vb_desc.InitialData = 0;

			ib_desc.Usage = BUFFER_USAGE_STREAM;
			ib_desc.InitialData = 0;

			renderer.CreateResource(vb_desc, &vertex_buffer);
			renderer.CreateResource(ib_desc, &index_buffer);

			unsigned char * vertex_data = (unsigned char *) renderer.Map(&vertex_buffer, MAP_WRITE_INVALIDATE_ALL);
			torus.GetPositions(vertex_data, byte_stride);
			torus.GetNormals(vertex_data + sizeof(float) * 3, byte_stride);
			torus.GetTexcoord2D(vertex_data + sizeof(float) * 6, byte_stride);
			renderer.Unmap(&vertex_buffer);
			
			unsigned char * index_data = (unsigned char *) renderer.Map(&index_buffer, MAP_WRITE_INVALIDATE_ALL);
			torus.GetTriangles(index_data);
			renderer.Unmap(&index_buffer);
		}

		DescVertexFormat vertex_format;
		vertex_format.format_entries.Set(3);
		vertex_format.format_entries[0].name = "POSITION";
		vertex_format.format_entries[0].type = 0;
		vertex_format.format_entries[0].byte_stride = byte_stride;
		vertex_format.format_entries[0].channels = 3;
		vertex_format.format_entries[0].offset = 0;
		vertex_format.format_entries[0].stream_index = 0;

		vertex_format.format_entries[1].name = "NORMAL";
		vertex_format.format_entries[1].type = 0;
		vertex_format.format_entries[1].byte_stride = byte_stride;
		vertex_format.format_entries[1].channels = 3;
		vertex_format.format_entries[1].offset = sizeof(float) * 3;
		vertex_format.format_entries[1].stream_index = 0;


		vertex_format.format_entries[2].name = "TEXCOORD";
		vertex_format.format_entries[2].type = 0;
		vertex_format.format_entries[2].byte_stride = byte_stride;
		vertex_format.format_entries[2].channels = 2;
		vertex_format.format_entries[2].offset = sizeof(float) * 6;
		vertex_format.format_entries[2].stream_index = 0;

		vertex_buffer.format_hash = renderer.GetVertexFormatHash(vertex_format);
		renderer.CreateInputLayout(vertex_buffer.format_hash, shader_program);

	}

	void Mesh_Torus::Deinit(RendererBase & renderer)
	{
		renderer.Delete(&vertex_buffer);
		renderer.Delete(&index_buffer);
	}

	void Mesh_Torus::PreRender(RendererBase & renderer, ShaderProgram * shader_program)
	{
		if (!static_data)
		{
			unsigned char * vertex_data = (unsigned char *) renderer.Map(&vertex_buffer, MAP_WRITE_INVALIDATE_ALL);
			torus.GetPositions(vertex_data, byte_stride);
			torus.GetNormals(vertex_data + sizeof(float) * 3, byte_stride);
			torus.GetTexcoord2D(vertex_data + sizeof(float) * 6, byte_stride);
			renderer.Unmap(&vertex_buffer);
			
			unsigned char * index_data = (unsigned char *) renderer.Map(&index_buffer, MAP_WRITE_INVALIDATE_ALL);
			torus.GetTriangles(index_data);
			renderer.Unmap(&index_buffer);
		}

		renderer.SetInputLayout(vertex_buffer.format_hash, shader_program->signature_hash);
		renderer.BindVertexBuffer(&vertex_buffer);
		renderer.BindIndexBuffer(&index_buffer);
	}

	void PostRender(RendererBase & renderer)
	{
		//renderer.Bind((VertexBuffer *) 0);
		//renderer.Bind((IndexBuffer *) 0);
	}
};


#endif