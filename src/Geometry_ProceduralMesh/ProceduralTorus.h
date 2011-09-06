#ifndef _PROCEDURAL_TORUS_H_
#define _PROCEDURAL_TORUS_H_

class ProceduralTorus
{
	unsigned int rings_count;
	unsigned int sides_count;
	float radius_inner;
	float radius_outer;
	bool texcoord2d;
	float rgba0[4];
	float rgba1[4];
	bool flip_coords;
public:
	void SetOptions(unsigned int _ring_count, unsigned int _side_count, float _radius_inner, float _radius_outer, bool _texcoord2d, bool flip_coords);
	void SetColors(float r0, float g0, float b0, float a0, float r1, float g1, float b1, float a1);
	void Normalize(float vect[3]);
	void GetPositions(void * dest, unsigned int byte_stride);
	void GetColors(void * dest, unsigned int byte_stride);
	void GetNormals(void * dest, unsigned int byte_stride);
	void GetTexcoord2D(void * dest, unsigned int byte_stride);
	void GetBitangents(void * dest, unsigned int byte_stride);
	void GetTriangles(void * dest);
	void GetPoints(void * dest);
	unsigned int GetVertexCount();
	unsigned int GetTriangleIndexCount();
};

#endif
