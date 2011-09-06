#ifndef _RENDERER_BASE_MESH_H_
#define _RENDERER_BASE_MESH_H_

#include "ExLib_RendererBase.h"
#include <Core_FixedArray.h>

class RendererBaseMesh
{
	DescVertexFormat desc_vertex_format;
	DescVertexBuffer desc_vertex_buffer;
	DescIndexBuffer desc_index_buffer;
	FixedArray<unsigned char> vertex_data;
	FixedArray<unsigned char> index_data;
public:
	inline DescVertexFormat & GetFormatDesc();
	inline DescVertexBuffer & GetVertexDesc();
	inline DescIndexBuffer & GetIndexDesc();
	inline FixedArray<unsigned char> & GetVertexData();
	inline FixedArray<unsigned char> & GetIndexData();
};

inline DescVertexFormat & RendererBaseMesh::GetFormatDesc()
{
	return desc_vertex_format;
}
inline DescVertexBuffer & RendererBaseMesh::GetVertexDesc()
{
	return desc_vertex_buffer;
}
inline DescIndexBuffer & RendererBaseMesh::GetIndexDesc()
{
	return desc_index_buffer;
}
inline FixedArray<unsigned char> & RendererBaseMesh::GetVertexData()
{
	return vertex_data;
}
inline FixedArray<unsigned char> & RendererBaseMesh::GetIndexData()
{
	return index_data;
}

#endif