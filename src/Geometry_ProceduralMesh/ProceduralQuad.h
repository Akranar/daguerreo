#ifndef _PROCEDURAL_QUAD_H_
#define _PROCEDURAL_QUAD_H_

#include "ExLib_Eigen.h"

class ProceduralQuad
{
	Eigen::Vector3f corners[4];
	unsigned int horizontal_sections;
	unsigned int vertical_sections;
	float rgba0[4];
	float rgba1[4];
	float flip_coords;
public:
	void SetOptions(bool flip_coords);
	void SetCorners(Eigen::Vector3f top_left, Eigen::Vector3f bottom_left, Eigen::Vector3f bottom_right, Eigen::Vector3f top_right);
	void SetSections(unsigned int horizontal, unsigned int vertical);
	void SetColors(float r0, float g0, float b0, float a0, float r1, float g1, float b1, float a1);
	unsigned int GetVertexCount();
	unsigned int GetIndexCount();
	void GetPositions(void * data_start, unsigned int byte_stride);
	void GetColors(void * data_start, unsigned int byte_stride);
	void GetNormals(void * data_start, unsigned int byte_stride);
	void GetTexcoord1D_U(void * data_start, unsigned int byte_stride);
	void GetTexcoord1D_V(void * data_start, unsigned int byte_stride);
	void GetTexcoord2D(void * data_start, unsigned int byte_stride);
	void GetTriangleList(void * data_start);
	void GetTriangleStrip(void * data_start);

	void GetTerrainStripIndices(void * data_start);
	unsigned int GetTerrainStripIndexCount();

	unsigned int GetTriangleListIndexCount();
	unsigned int GetTriangleStripIndexCount();
	
};

#endif
