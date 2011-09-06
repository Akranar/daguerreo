#include "ProceduralTorus.h"
#include <cmath>

//******************************************************************************************
//	Notes:
//		-> Definitions:
//			-> The torus: 
//				-> imagine a tube bent into a circle and its two ends connected.
//			-> Ring: The circle you get from taking a cross section of the tube.
//			-> Side: The number of sides each ring has.
//				-> As # of sides approach infinity, Ring becomes a circle.
//			-> Torus origin:
//				-> Located at the right-most point along the inner radius of the torus:
//					-> Marked by X in figure:
//                  _________
//                 /   ___   \
//                /  /     \  \
//               |  |       X  |
//                \  \ ___ /  /
//                 \ _______ /
//              
//				-> U, V = (0, 0) at origin.
//					-> U increases as we go around a ring (from the front)
//					-> V increases as we process rings in counter-clockwise order.
//                  
//		-> Algorithm for Vertex Generation:
//			-> Process ring by ring in counter-clockwise order.
//				-> Traces unit-circle
//					-> Track progress around torus with angle THETA
//						-> (x, y) = (cos(THETA) * R, sin(THETA) * R)
//				-> For each ring, process each side.
//					-> Can track our process around ring with angle PHI
//
//*******************************************************************************************

void ProceduralTorus::SetOptions(unsigned int _ring_count, unsigned int _side_count, float _radius_inner, float _radius_outer, bool _texcoord2d, bool _flip_coords)
{
	rings_count = _ring_count;
	sides_count = _side_count;
	radius_inner = _radius_inner;
	radius_outer = _radius_outer;
	//Note: If have texcoord2d's, must add seams at r = 0 or R and s = 0 or S.
	texcoord2d = _texcoord2d;
	flip_coords = _flip_coords;
}
inline void ProceduralTorus::Normalize(float vect[3])
{
	float length_sq = vect[0] * vect[0] + vect[1] * vect[1] + vect[2] * vect[2];
	if (length_sq == 0) return;
	float length = sqrt(length_sq);
	if (length - 1 < 1e-20) return;
	vect[0] /= length;
	vect[1] /= length;
	vect[2] /= length;
}

void ProceduralTorus::GetPositions(void * dest, unsigned int byte_stride)
{
	#define PI 3.14159265358979323846f
	const float TWO_PI = PI * 2;
	const float HALF_PI = PI / 2;
	const float torus_width = radius_outer - radius_inner;

	float * position_dest = (float *) dest;

	//Note: If we have texcoord2ds, we need the vertices at side "s" = S to be different from "s" = 0.
	//	-> Same for vertices along rings.
	//	-> Add 1 to num_verts for s and r.
	unsigned int num_verts_per_ring = texcoord2d ? (rings_count + 1) : rings_count;
	unsigned int num_verts_per_side = texcoord2d ? (sides_count + 1) : sides_count;

	for (unsigned int r = 0; r < num_verts_per_ring; ++r)
	{
		float v = float(r) / float(rings_count);
		float theta = v * TWO_PI;
		float x_inner = radius_inner * cos(theta);
		float x_outer = radius_outer * cos(theta);
		float y_inner = radius_inner * sin(theta);
		float y_outer = radius_outer * sin(theta);

		float x_center = (x_outer + x_inner) * 0.5f;
		float y_center = (y_outer + y_inner) * 0.5f;
		for (unsigned int s = 0; s < num_verts_per_side; ++s)
		{
			float u = float(s) / float(sides_count);
			float phi = u * TWO_PI;

			//float x = radius_inner * cos(theta) + cos(theta) * sin(phi * 0.5f) * torus_width;
			//float y = radius_inner * sin(theta) + sin(theta) * sin(phi * 0.5f) * torus_width;
			//float x = x_inner + (x_outer - x_inner) * sin(phi * 0.5f);
			//float y = y_inner + (y_outer - y_inner) * sin(phi * 0.5f);
			float xy_sin = sin(phi - HALF_PI);
			float x = x_center + (xy_sin * (x_outer - x_inner) * 0.5f);
			float y = y_center + (xy_sin * (y_outer - y_inner) * 0.5f);
			float z = sin(phi) * torus_width * 0.5f;

			position_dest[0] = x;
			position_dest[1] = y;
			position_dest[2] = z;
			position_dest = (float *) (((char *) position_dest) + byte_stride);
		}
	}
}

void ProceduralTorus::SetColors(float r0, float g0, float b0, float a0, float r1, float g1, float b1, float a1)
{
	rgba0[0] = r0;
	rgba0[1] = g0;
	rgba0[2] = b0;
	rgba0[3] = a0;
	rgba1[0] = r1;
	rgba1[1] = g1;
	rgba1[2] = b1;
	rgba1[3] = a1;
}

void ProceduralTorus::GetColors(void * dest, unsigned int byte_stride)
{
	float * colors_dest = (float *) dest;

	//Note: If we have texcoord2ds, we need the vertices at side "s" = S to be different from "s" = 0.
	//	-> Same for vertices along rings.
	//	-> Add 1 to num_verts for s and r.
	unsigned int num_verts_per_ring = texcoord2d ? (rings_count + 1) : rings_count;
	unsigned int num_verts_per_side = texcoord2d ? (sides_count + 1) : sides_count;

	for (unsigned int r = 0; r < num_verts_per_ring; ++r)
	{
		float v = float(r) / float(rings_count);
		for (unsigned int s = 0; s < num_verts_per_side; ++s)
		{
			float u = float(s) / float(sides_count);
			colors_dest[0] = (1-u) * rgba0[0] + u * rgba1[0];
			colors_dest[1] = (1-u) * rgba0[1] + u * rgba1[1];
			colors_dest[2] = (1-u) * rgba0[2] + u * rgba1[2];
			colors_dest[3] = (1-u) * rgba0[3] + u * rgba1[3];

			colors_dest = (float *) (((char *) colors_dest) + byte_stride);
		}
	}
}

void ProceduralTorus::GetNormals(void * dest, unsigned int byte_stride)
{
	#define PI 3.14159265358979323846f
	const float TWO_PI = PI * 2;
	const float HALF_PI = PI / 2;
	const float torus_width = radius_outer - radius_inner;

	float * normal_dest = (float *) dest;

	//Note: If we have texcoord2ds, we need the vertices at side "s" = S to be different from "s" = 0.
	//	-> Same for vertices along rings.
	//	-> Add 1 to num_verts for s and r.
	unsigned int num_verts_per_ring = texcoord2d ? (rings_count + 1) : rings_count;
	unsigned int num_verts_per_side = texcoord2d ? (sides_count + 1) : sides_count;

	for (unsigned int r = 0; r < num_verts_per_ring; ++r)
	{
		float v = float(r) / float(rings_count);
		float theta = v * TWO_PI;
		float x_inner = radius_inner * cos(theta);
		float x_outer = radius_outer * cos(theta);
		float y_inner = radius_inner * sin(theta);
		float y_outer = radius_outer * sin(theta);

		float torus_center_x = (x_inner + x_outer) * 0.5f;
		float torus_center_y = (y_inner + y_outer) * 0.5f;
		float torus_center_z = 0.0f;

		for (unsigned int s = 0; s < num_verts_per_side; ++s)
		{
			float u = float(s) / float(sides_count);
			float phi = u * TWO_PI;

			//float x = radius_inner * cos(theta) + cos(theta) * sin(phi * 0.5f) * torus_width;
			//float y = radius_inner * sin(theta) + sin(theta) * sin(phi * 0.5f) * torus_width;
			float x = x_inner + (x_outer - x_inner) * sin(phi * 0.5f);
			float y = y_inner + (y_outer - y_inner) * sin(phi * 0.5f);
			float z = sin(phi) * torus_width * 0.5f;

			float normal[3];
			normal[0] = x - torus_center_x;
			normal[1] = y - torus_center_y;
			normal[2] = z - torus_center_z;

			Normalize(normal);

			normal_dest[0] = normal[0];
			normal_dest[1] = normal[1];
			normal_dest[2] = normal[2];
			normal_dest = (float *) (((char *) normal_dest) + byte_stride);
		}
	}
}

void ProceduralTorus::GetTexcoord2D(void * dest, unsigned int byte_stride)
{
	float * texcoord2d_dest = (float *) dest;

	//Note: If we have texcoord2ds, we need the vertices at side "s" = S to be different from "s" = 0.
	//	-> Same for vertices along rings.
	//	-> Add 1 to num_verts for s and r.
	unsigned int num_verts_per_ring = texcoord2d ? (rings_count + 1) : rings_count;
	unsigned int num_verts_per_side = texcoord2d ? (sides_count + 1) : sides_count;

	for (unsigned int r = 0; r < num_verts_per_ring; ++r)
	{
		float v = float(r) / float(rings_count);
		for (unsigned int s = 0; s < num_verts_per_side; ++s)
		{
			float u = float(s) / float(sides_count);

			if (flip_coords)
			{
				texcoord2d_dest[0] = u;
				texcoord2d_dest[1] = 1-v;	
			}
			else
			{
				texcoord2d_dest[0] = u;
				texcoord2d_dest[1] = v;
			}
			texcoord2d_dest = (float *) (((char *) texcoord2d_dest) + byte_stride);
		}
	}
}

void ProceduralTorus::GetBitangents(void * dest, unsigned int byte_stride)
{
	#define PI 3.14159265358979323846f
	const float TWO_PI = PI * 2;
	const float HALF_PI = PI / 2;
	const float torus_width = radius_outer - radius_inner;

	float * bitangent_dest = (float *) dest;

	//Note: If we have texcoord2ds, we need the vertices at side "s" = S to be different from "s" = 0.
	//	-> Same for vertices along rings.
	//	-> Add 1 to num_verts for s and r.
	unsigned int num_verts_per_ring = texcoord2d ? (rings_count + 1) : rings_count;
	unsigned int num_verts_per_side = texcoord2d ? (sides_count + 1) : sides_count;

	for (unsigned int r = 0; r < num_verts_per_ring; ++r)
	{
		float v0 = float(r) / float(rings_count);
		float theta0 = v0 * TWO_PI;
		float x_inner0 = radius_inner * cos(theta0);
		float x_outer0 = radius_outer * cos(theta0);
		float y_inner0 = radius_inner * sin(theta0);
		float y_outer0 = radius_outer * sin(theta0);

		float v1 = float((r+1) % num_verts_per_ring) / float(rings_count);
		float theta1 = v1 * TWO_PI;
		float x_inner1 = radius_inner * cos(theta1);
		float x_outer1 = radius_outer * cos(theta1);
		float y_inner1 = radius_inner * sin(theta1);
		float y_outer1 = radius_outer * sin(theta1);
		for (unsigned int s = 0; s < num_verts_per_side; ++s)
		{
			float u = float(s) / float(sides_count);
			float phi = u * TWO_PI;

			float x0 = x_inner0 + (x_outer0 - x_inner0) * sin(phi * 0.5f);
			float y0 = y_inner0 + (y_outer0 - y_inner0) * sin(phi * 0.5f);
			float z0 = sin(phi) * torus_width * 0.5f;

			float x1 = x_inner1 + (x_outer1 - x_inner1) * sin(phi * 0.5f);
			float y1 = y_inner1 + (y_outer1 - y_inner1) * sin(phi * 0.5f);
			float z1 = z0;

			float binormal[3];
			binormal[0] = x1-x0;
			binormal[1] = y1-y0;
			binormal[2] = z1-z0;

			Normalize(binormal);

			bitangent_dest[0] = binormal[0];
			bitangent_dest[1] = binormal[1];
			bitangent_dest[2] = binormal[2];
			bitangent_dest = (float *) (((char *) bitangent_dest) + byte_stride);
		}
	}
}


void ProceduralTorus::GetTriangles(void * dest)
{
	unsigned int * index_dest = (unsigned int *) dest;

	//Note A: If we have texcoord2ds, we need the vertices at side "s" = S to be different from "s" = 0.
	//	-> Same for vertices along rings.
	//	-> Add 1 to num_verts for s and r.
	//		-> Will still have the same number of indices in the end,
	//			-> The difference is whether our last index for r is R or 0 and last index for s is S or 0.
	//				-> Determined by the mod with num_verts (s and r).
	unsigned int num_verts_per_ring = texcoord2d ? (rings_count + 1) : rings_count;
	unsigned int num_verts_per_side = texcoord2d ? (sides_count + 1) : sides_count;
	for (unsigned int r = 0; r < rings_count; ++r)
	{
		unsigned int next_r = (r + 1) % num_verts_per_ring;
		for (unsigned int s = 0; s < sides_count; ++s)
		{
			unsigned int next_s = (s + 1) % num_verts_per_side;

			unsigned int index_a = s + num_verts_per_side * r;
			unsigned int index_b = next_s + num_verts_per_side * r;
			unsigned int index_c = next_s + num_verts_per_side * next_r;
			unsigned int index_d = s + num_verts_per_side * next_r;
			//Note: Two Triangles
			//	-> a, b, d
			//	-> d, b, c
			index_dest[0] = index_a;
			index_dest[1] = index_b;
			index_dest[2] = index_d;
			index_dest[3] = index_d;
			index_dest[4] = index_b;
			index_dest[5] = index_c;

			index_dest += 6;
		}
	}
}

void ProceduralTorus::GetPoints(void * dest)
{
	unsigned int * index_dest = (unsigned int *) dest;

	unsigned int vert_count = GetVertexCount();
	for (unsigned int i = 0; i < vert_count; ++i)
	{
		index_dest[0] = i;
		index_dest += 1;
	}
}

unsigned int ProceduralTorus::GetVertexCount()
{
	//Note: If we have texcoord2ds, we need the vertices at side "s" = S to be different from "s" = 0.
	//	-> Same for vertices along rings.
	//	-> Add 1 to num_verts for s and r.
	unsigned int num_verts_per_ring = texcoord2d ? (rings_count + 1) : rings_count;
	unsigned int num_verts_per_side = texcoord2d ? (sides_count + 1) : sides_count;
	return num_verts_per_ring * num_verts_per_side;
}

unsigned int ProceduralTorus::GetTriangleIndexCount()
{
	//Note:
	//	-> # of quads = rings * sides.
	//	-> 6 indices per quad.
	return rings_count * sides_count *6;
}
