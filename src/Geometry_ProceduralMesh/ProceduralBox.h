#ifndef _PROCEDURAL_BOX_H_
#define _PROCEDURAL_BOX_H_

#include "ExLib_Eigen.h"

class ProceduralBox
{
	unsigned int width, length, height;
	bool flip_coords;
	bool has_texcoords;
	bool has_normals;
	bool inside_out;
public:
	void SetOptions(unsigned int _width, unsigned int _length, unsigned int _height, bool _has_normals, bool _has_texcoords, bool flip_coords, bool _inside_out);

	void GetPositions(void * dest, unsigned int byte_stride);
	void GetNormals(void * dest, unsigned int byte_stride);
	void GetTexcoord2D(void * dest, unsigned int byte_stride);
	void GetTriangleListIndices(void * data_start);

	unsigned int GetVertexCount();
	unsigned int GetTriangleListIndexCount();
};

#endif