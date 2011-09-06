#include "ProceduralBox.h"

void ProceduralBox::SetOptions(unsigned int _width, unsigned int _length, unsigned int _height, bool _has_normals, bool _has_texcoords, bool _flip_coords, bool _inside_out)
{
	width = _width;
	length = _length;
	height = _height;
	flip_coords = _flip_coords;
	has_texcoords = _has_texcoords;
	has_normals = _has_normals;
	inside_out = _inside_out;
}

void ProceduralBox::GetPositions(void * dest, unsigned int byte_stride)
{
	float * float_dest = (float *) dest;
	float half_width = width * 0.5f;
	float half_length = length * 0.5f;
	float half_height = height * 0.5f;

	unsigned int component_index[3];
	if (inside_out)
	{
		component_index[0] = 2;
		component_index[1] = 1;
		component_index[2] = 0;
	}
	else
	{
		component_index[0] = 0;
		component_index[1] = 1;
		component_index[2] = 2;
	}

	{//Front Face. Start with Lower Left Corner for All faces.
		float_dest[component_index[0]] = -half_width; float_dest[component_index[1]] = -half_length; float_dest[component_index[2]] = -half_height; 
		float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
		float_dest[component_index[0]] = half_width; float_dest[component_index[1]] = -half_length; float_dest[component_index[2]] = -half_height;
		float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
		float_dest[component_index[0]] = half_width; float_dest[component_index[1]] = -half_length; float_dest[component_index[2]] = half_height;
		float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
		float_dest[component_index[0]] = -half_width; float_dest[component_index[1]] = -half_length; float_dest[component_index[2]] = half_height;
		float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
	}
	{//Back Face
		float_dest[component_index[0]] = half_width; float_dest[component_index[1]] = half_length; float_dest[component_index[2]] = -half_height; 
		float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
		float_dest[component_index[0]] = -half_width; float_dest[component_index[1]] = half_length; float_dest[component_index[2]] = -half_height;
		float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
		float_dest[component_index[0]] = -half_width; float_dest[component_index[1]] = half_length; float_dest[component_index[2]] = half_height;
		float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
		float_dest[component_index[0]] = half_width; float_dest[component_index[1]] = half_length; float_dest[component_index[2]] = half_height;
		float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
	}
	if (has_texcoords || has_normals)
	{
		{//Left Face
			float_dest[component_index[0]] = -half_width; float_dest[component_index[1]] = half_length; float_dest[component_index[2]] = -half_height; 
			float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
			float_dest[component_index[0]] = -half_width; float_dest[component_index[1]] = -half_length; float_dest[component_index[2]] = -half_height; 
			float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
			float_dest[component_index[0]] = -half_width; float_dest[component_index[1]] = -half_length; float_dest[component_index[2]] = half_height; 
			float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
			float_dest[component_index[0]] = -half_width; float_dest[component_index[1]] = half_length; float_dest[component_index[2]] = half_height; 
			float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
		}
		{//Right Face
			float_dest[component_index[0]] = half_width; float_dest[component_index[1]] = -half_length; float_dest[component_index[2]] = -half_height; 
			float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
			float_dest[component_index[0]] = half_width; float_dest[component_index[1]] = half_length; float_dest[component_index[2]] = -half_height; 
			float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
			float_dest[component_index[0]] = half_width; float_dest[component_index[1]] = half_length; float_dest[component_index[2]] = half_height; 
			float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
			float_dest[component_index[0]] = half_width; float_dest[component_index[1]] = -half_length; float_dest[component_index[2]] = half_height; 
			float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
		}
		{//Top Face. Start with Corner connected (by edge) to the start corner of Front Face.
			float_dest[component_index[0]] = -half_width; float_dest[component_index[1]] = -half_length; float_dest[component_index[2]] = half_height; 
			float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
			float_dest[component_index[0]] = half_width; float_dest[component_index[1]] = -half_length; float_dest[component_index[2]] = half_height; 
			float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
			float_dest[component_index[0]] = half_width; float_dest[component_index[1]] = half_length; float_dest[component_index[2]] = half_height; 
			float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
			float_dest[component_index[0]] = -half_width; float_dest[component_index[1]] = half_length; float_dest[component_index[2]] = half_height; 
			float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
		}
		{//Bottom Face. Start with Corner connected (by edge) to the start corner of Front Face.
			float_dest[component_index[0]] = -half_width; float_dest[component_index[1]] = -half_length; float_dest[component_index[2]] = -half_height; 
			float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
			float_dest[component_index[0]] = -half_width; float_dest[component_index[1]] = half_length; float_dest[component_index[2]] = -half_height; 
			float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
			float_dest[component_index[0]] = half_width; float_dest[component_index[1]] = half_length; float_dest[component_index[2]] = -half_height; 
			float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
			float_dest[component_index[0]] = half_width; float_dest[component_index[1]] = -half_length; float_dest[component_index[2]] = -half_height; 
			float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
		}
	}
}
void ProceduralBox::GetNormals(void * dest, unsigned int byte_stride)
{
	float * float_dest = (float *) dest;
	if (inside_out)
	{
		if (has_normals)
		{
			{//Front Face. Start with Lower Left Corner for All faces.
				float_dest[0] = 0.0f; float_dest[1] = -1.0f; float_dest[2] = 0.0f; 
				float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
				float_dest[0] = 0.0f; float_dest[1] = -1.0f; float_dest[2] = 0.0f; 
				float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
				float_dest[0] = 0.0f; float_dest[1] = -1.0f; float_dest[2] = 0.0f; 
				float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
				float_dest[0] = 0.0f; float_dest[1] = -1.0f; float_dest[2] = 0.0f; 
				float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
			}
			{//Back Face
				float_dest[0] = 0.0f; float_dest[1] = 1.0f; float_dest[2] = 0.0f; 
				float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
				float_dest[0] = 0.0f; float_dest[1] = 1.0f; float_dest[2] = 0.0f; 
				float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
				float_dest[0] = 0.0f; float_dest[1] = 1.0f; float_dest[2] = 0.0f; 
				float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
				float_dest[0] = 0.0f; float_dest[1] = 1.0f; float_dest[2] = 0.0f; 
				float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
			}
			{//Left Face
				float_dest[0] = 1.0f; float_dest[1] = 0.0f; float_dest[2] = 0.0f; 
				float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
				float_dest[0] = 1.0f; float_dest[1] = 0.0f; float_dest[2] = 0.0f; 
				float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
				float_dest[0] = 1.0f; float_dest[1] = 0.0f; float_dest[2] = 0.0f; 
				float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
				float_dest[0] = 1.0f; float_dest[1] = 0.0f; float_dest[2] = 0.0f; 
				float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
			}
			{//Right Face
				float_dest[0] = -1.0f; float_dest[1] = 0.0f; float_dest[2] = 0.0f; 
				float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
				float_dest[0] = -1.0f; float_dest[1] = 0.0f; float_dest[2] = 0.0f; 
				float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
				float_dest[0] = -1.0f; float_dest[1] = 0.0f; float_dest[2] = 0.0f; 
				float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
				float_dest[0] = -1.0f; float_dest[1] = 0.0f; float_dest[2] = 0.0f; 
				float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
			}
			{//Top Face. Start with Corner connected (by edge) to the start corner of Front Face.
				float_dest[0] = 0.0f; float_dest[1] = 0.0f; float_dest[2] = -1.0f; 
				float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
				float_dest[0] = 0.0f; float_dest[1] = 0.0f; float_dest[2] = -1.0f; 
				float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
				float_dest[0] = 0.0f; float_dest[1] = 0.0f; float_dest[2] = -1.0f; 
				float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
				float_dest[0] = 0.0f; float_dest[1] = 0.0f; float_dest[2] = -1.0f; 
				float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
			}
			{//Bottom Face. Start with Corner connected (by edge) to the start corner of Front Face.
				float_dest[0] = 0.0f; float_dest[1] = 0.0f; float_dest[2] = 1.0f; 
				float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
				float_dest[0] = 0.0f; float_dest[1] = 0.0f; float_dest[2] = 1.0f; 
				float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
				float_dest[0] = 0.0f; float_dest[1] = 0.0f; float_dest[2] = 1.0f; 
				float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
				float_dest[0] = 0.0f; float_dest[1] = 0.0f; float_dest[2] = 1.0f; 
				float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
			}
		}
	}
	else
	{
		if (has_normals)
		{
			{//Front Face. Start with Lower Left Corner for All faces.
				float_dest[0] = 0.0f; float_dest[1] = 1.0f; float_dest[2] = 0.0f; 
				float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
				float_dest[0] = 0.0f; float_dest[1] = 1.0f; float_dest[2] = 0.0f; 
				float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
				float_dest[0] = 0.0f; float_dest[1] = 1.0f; float_dest[2] = 0.0f; 
				float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
				float_dest[0] = 0.0f; float_dest[1] = 1.0f; float_dest[2] = 0.0f; 
				float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
			}
			{//Back Face
				float_dest[0] = 0.0f; float_dest[1] = -1.0f; float_dest[2] = 0.0f; 
				float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
				float_dest[0] = 0.0f; float_dest[1] = -1.0f; float_dest[2] = 0.0f; 
				float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
				float_dest[0] = 0.0f; float_dest[1] = -1.0f; float_dest[2] = 0.0f; 
				float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
				float_dest[0] = 0.0f; float_dest[1] = -1.0f; float_dest[2] = 0.0f; 
				float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
			}
			{//Left Face
				float_dest[0] = -1.0f; float_dest[1] = 0.0f; float_dest[2] = 0.0f; 
				float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
				float_dest[0] = -1.0f; float_dest[1] = 0.0f; float_dest[2] = 0.0f; 
				float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
				float_dest[0] = -1.0f; float_dest[1] = 0.0f; float_dest[2] = 0.0f; 
				float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
				float_dest[0] = -1.0f; float_dest[1] = 0.0f; float_dest[2] = 0.0f; 
				float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
			}
			{//Right Face
				float_dest[0] = 1.0f; float_dest[1] = 0.0f; float_dest[2] = 0.0f; 
				float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
				float_dest[0] = 1.0f; float_dest[1] = 0.0f; float_dest[2] = 0.0f; 
				float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
				float_dest[0] = 1.0f; float_dest[1] = 0.0f; float_dest[2] = 0.0f; 
				float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
				float_dest[0] = 1.0f; float_dest[1] = 0.0f; float_dest[2] = 0.0f; 
				float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
			}
			{//Top Face. Start with Corner connected (by edge) to the start corner of Front Face.
				float_dest[0] = 0.0f; float_dest[1] = 0.0f; float_dest[2] = 1.0f; 
				float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
				float_dest[0] = 0.0f; float_dest[1] = 0.0f; float_dest[2] = 1.0f; 
				float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
				float_dest[0] = 0.0f; float_dest[1] = 0.0f; float_dest[2] = 1.0f; 
				float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
				float_dest[0] = 0.0f; float_dest[1] = 0.0f; float_dest[2] = 1.0f; 
				float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
			}
			{//Bottom Face. Start with Corner connected (by edge) to the start corner of Front Face.
				float_dest[0] = 0.0f; float_dest[1] = 0.0f; float_dest[2] = -1.0f; 
				float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
				float_dest[0] = 0.0f; float_dest[1] = 0.0f; float_dest[2] = -1.0f; 
				float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
				float_dest[0] = 0.0f; float_dest[1] = 0.0f; float_dest[2] = -1.0f; 
				float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
				float_dest[0] = 0.0f; float_dest[1] = 0.0f; float_dest[2] = -1.0f; 
				float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
			}
		}
	}
	
}
void ProceduralBox::GetTexcoord2D(void * dest, unsigned int byte_stride)
{
	float * float_dest = (float *) dest;
	if (has_texcoords)
	{
		for (unsigned int i = 0; i < 6; ++i)
		{
			float_dest[0] = 0.0f; float_dest[1] = 0.0f; 
			float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
			float_dest[0] = 1.0f; float_dest[1] = 0.0f; 
			float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
			float_dest[0] = 1.0f; float_dest[1] = 1.0f; 
			float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
			float_dest[0] = 0.0f; float_dest[1] = 0.0f; 
			float_dest = (float *) (((unsigned char *) float_dest) + byte_stride);
		}
	}
}
void ProceduralBox::GetTriangleListIndices(void * data_start)
{
	unsigned int * index_dest = (unsigned int *) data_start;

	if (has_texcoords || has_normals)
	{
		unsigned int face_start_indices[6];

		face_start_indices[0] = 0; //Front face
		face_start_indices[1] = 4; //Back
		face_start_indices[2] = 8; //Left
		face_start_indices[3] = 12; //Right
		face_start_indices[4] = 16; //Top
		face_start_indices[5] = 20; //Bottom

		unsigned int index = 0;
		for (unsigned int i = 0; i < 6; ++i)
		{
			index_dest[index++] = face_start_indices[i] + 0;
			index_dest[index++] = face_start_indices[i] + 1;
			index_dest[index++] = face_start_indices[i] + 3;
			index_dest[index++] = face_start_indices[i] + 1;
			index_dest[index++] = face_start_indices[i] + 2;
			index_dest[index++] = face_start_indices[i] + 3;
		}
	}
	else
	{
		unsigned int index = 0;
		{//Front
			index_dest[index++] = 0;
			index_dest[index++] = 1;
			index_dest[index++] = 3;
			index_dest[index++] = 1;
			index_dest[index++] = 2;
			index_dest[index++] = 3;
		}
		{//Right
			index_dest[index++] = 1;
			index_dest[index++] = 4;
			index_dest[index++] = 2;
			index_dest[index++] = 4;
			index_dest[index++] = 7;
			index_dest[index++] = 2;
		}
		{//Back
			index_dest[index++] = 4;
			index_dest[index++] = 5;
			index_dest[index++] = 7;
			index_dest[index++] = 5;
			index_dest[index++] = 6;
			index_dest[index++] = 7;
		}
		{//Left
			index_dest[index++] = 5;
			index_dest[index++] = 0;
			index_dest[index++] = 3;
			index_dest[index++] = 6;
			index_dest[index++] = 5;
			index_dest[index++] = 3;
		}
		{//Top
			index_dest[index++] = 3;
			index_dest[index++] = 2;
			index_dest[index++] = 7;
			index_dest[index++] = 6;
			index_dest[index++] = 3;
			index_dest[index++] = 7;
		}
		{//Bottom
			index_dest[index++] = 4;
			index_dest[index++] = 1;
			index_dest[index++] = 0;
			index_dest[index++] = 4;
			index_dest[index++] = 0;
			index_dest[index++] = 5;
		}
	}
}

unsigned int ProceduralBox::GetVertexCount()
{
	if (has_texcoords || has_normals)
	{
		return 24;
	}
	else
	{
		return 8;
	}
}
unsigned int ProceduralBox::GetTriangleListIndexCount()
{
	return 36;
}