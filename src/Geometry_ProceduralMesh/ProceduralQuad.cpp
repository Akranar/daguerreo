
#include "ProceduralQuad.h"
#include <iostream>

void ProceduralQuad::SetOptions(bool _flip_coords)
{
	flip_coords = _flip_coords;
}

void ProceduralQuad::SetCorners(Eigen::Vector3f bottom_left, Eigen::Vector3f bottom_right, Eigen::Vector3f top_right, Eigen::Vector3f top_left)
{
	corners[0] = bottom_left;
	corners[1] = bottom_right;
	corners[2] = top_right;
	corners[3] = top_left;
}
void ProceduralQuad::SetSections(unsigned int horizontal, unsigned int vertical)
{
	horizontal_sections = horizontal;
	vertical_sections = vertical;
}
void ProceduralQuad::SetColors(float r0, float g0, float b0, float a0, float r1, float g1, float b1, float a1)
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
void ProceduralQuad::GetColors(void * data_start, unsigned int byte_stride)
{
	float * dest = (float *) data_start;
	unsigned int rows = vertical_sections+1;
	unsigned int columns = horizontal_sections+1;
	for (unsigned int i = 0; i < rows; ++i)
	{
		//float v = float(i) / float(vertical_sections); //Down to Up
		for (unsigned int j = 0; j < columns; ++j)
		{
			float u = float(horizontal_sections-j) / float(horizontal_sections); //Right to Left to match positions.
			dest[0] = (1-u) * rgba0[0] + u * rgba1[0];
			dest[1] = (1-u) * rgba0[1] + u * rgba1[1];
			dest[2] = (1-u) * rgba0[2] + u * rgba1[2];
			dest[3] = (1-u) * rgba0[3] + u * rgba1[3];
			dest = (float *) ((char *) dest + byte_stride);
		}
	}
}
unsigned int ProceduralQuad::GetVertexCount()
{
	return (vertical_sections + 1) * (horizontal_sections + 1);
}
unsigned int ProceduralQuad::GetIndexCount()
{
	return vertical_sections * horizontal_sections * 2;
}
void ProceduralQuad::GetPositions(void * data_start, unsigned int byte_stride)
{
	float * dest = (float *) data_start;
	unsigned int rows = vertical_sections+1;
	unsigned int columns = horizontal_sections+1;
	for (unsigned int i = 0; i < rows; ++i)
	{
		float vertical_factor = float(i) / float(vertical_sections); //Down to Up
		Eigen::Vector3f left_interp = corners[0] * (1-vertical_factor) + corners[3] * (vertical_factor);
		Eigen::Vector3f right_interp = corners[1] * (1-vertical_factor) + corners[2] * (vertical_factor);
		for (unsigned int j = 0; j < columns; ++j)
		{
			float horizontal_factor = float(horizontal_sections-j) / float(horizontal_sections); //Right to Left
			//float horizontal_factor = float(j) / float(horizontal_sections); //Left to Right
			Eigen::Vector3f final_interp = left_interp * (1-horizontal_factor) + right_interp * (horizontal_factor);
			dest[0] = final_interp[0];
			dest[1] = final_interp[1];
			dest[2] = final_interp[2];
			dest = (float *) ((char *) dest + byte_stride);
		}
	}
}
void ProceduralQuad::GetNormals(void * data_start, unsigned int byte_stride)
{
	Eigen::Vector3f normal = (corners[1]-corners[0]).cross(corners[2]-corners[0]);
	normal.normalize();

	float * dest = (float *) data_start;

	unsigned int vertex_count = GetVertexCount();
	for (unsigned int i = 0; i < vertex_count; ++i)
	{
		dest[0] = normal[0];
		dest[1] = normal[1];
		dest[2] = normal[2];
		dest = (float *) ((char *) dest + byte_stride);
	}
}
void ProceduralQuad::GetTexcoord1D_U(void * data_start, unsigned int byte_stride)
{
	float * dest = (float *) data_start;
	unsigned int rows = vertical_sections+1;
	unsigned int columns = horizontal_sections+1;
	for (unsigned int i = 0; i < rows; ++i)
	{
		for (unsigned int j = 0; j < columns; ++j)
		{
			float horizontal_factor = float(horizontal_sections-j) / float(horizontal_sections); //Right to Left to match positions.
			dest[0] = horizontal_factor;
			dest = (float *) ((char *) dest + byte_stride);
		}
	}
}
void ProceduralQuad::GetTexcoord1D_V(void * data_start, unsigned int byte_stride)
{
	float * dest = (float *) data_start;
	unsigned int rows = vertical_sections+1;
	unsigned int columns = horizontal_sections+1;
	for (unsigned int i = 0; i < rows; ++i)
	{
		float vertical_factor = float(i) / float(vertical_sections); //Down to Up
		for (unsigned int j = 0; j < columns; ++j)
		{
			dest[0] = vertical_factor;
			dest = (float *) ((char *) dest + byte_stride);
		}
	}
}
void ProceduralQuad::GetTexcoord2D(void * data_start, unsigned int byte_stride)
{
	float * dest = (float *) data_start;
	unsigned int rows = vertical_sections+1;
	unsigned int columns = horizontal_sections+1;
	for (unsigned int i = 0; i < rows; ++i)
	{
		float vertical_factor = float(i) / float(vertical_sections); //Down to Up
		for (unsigned int j = 0; j < columns; ++j)
		{
			float horizontal_factor = float(horizontal_sections-j) / float(horizontal_sections); //Right to Left to match positions.
			dest[0] = horizontal_factor;
			dest[1] = vertical_factor;
			dest = (float *) ((char *) dest + byte_stride);
		}
	}
}
void ProceduralQuad::GetTriangleList(void * data_start)
{

}

//*****************************************************************************
//Note: Triangle Strip Algorithm for Sectioned Quad.
//	-> Vertices are numbered like this in Vertex Buffer:
//		Example for (H, V) = (4, 2):
//			14-------13-------12-------11-------10	
//          |        |        |        |        |
//          9--------8--------7--------6--------5
//          |        |        |        |        |
//          4--------3--------2--------1--------0
//
//	-> Strip like so:
//		-> First row:
//			-> 0, 5,, 1, 6,, 2, 7,, 3, 8,, 4, 9,
//		-> Then add Degenerate Index:
//			-> 9, 
//		-> Continue with second row:
//			-> 9, 14,, 8, 13,, 7, 12,, 6, 11,, 5, 10
//	-> Notice the indices come in pairs:
//		-> First row: (0, 5), (1, 6) ...
//		-> Second Row: (9, 14), (8, 13) ...
//		-> Third Row: (10, 15), (11, 16) ...
//		-> Fourth Row: (14, 19), (13, 18) ...
//	-> Insights:
//		-> Define (C, R) = (H+1, V+1)
//		-> Start pair in EVEN rows = (row_index * C, (row_index+1) * C)
//			-> In example: 
//				-> H = 4, C = H+1 = 5
//				-> start pair in row 0 = (0 * 5, 1 * 5) = (0, 5)
//				-> start pair in row 2 = (2 * 5, 3 * 5) = (10, 15)
//		-> Start pair in ODD rows = (H+row_index*C, H+(row_index+1)*C)
//			-> In example:
//				-> start pair in row 1 = (4+1*5, 4+2*5) = (9, 14)
//				-> start pair in row 3 = (4+2*5, 4+3*5) = (14, 19)
//		-> End an EVEN row when current.A = start.A+C
//			-> Then add degenerates with current.B-1
//				-> In example:
//					-> Stop Row 0 when we hit pair (5, 10)
//					-> Append degenerate index: 10-1 = 9
//		-> End an ODD row when current.A = start.A-C
//			-> Then add degenerates with current.B+1
//				-> In example:
//					-> Stop Row 1 when we hit pair (4, 9)
//					-> Append degenerate index: 9+1 = 10
//		-> Even Rows: (row 0, row 2, row 4, row 6 ..)
//			-> Increment.
//				-> (c, d) = (a+1, b+1)
//					-> where (c, d) is next pair and (a, b) is previous pair.
//		-> Odd Rows:
//			-> Decrement
//				-> (c, d) = (a-1, b-1)
//					-> where (c, d) is next pair and (a, b) is previous pair.
//
//*****************************************************************************

void ProceduralQuad::GetTriangleStrip(void * data_start)
{
	unsigned int * dest = (unsigned int *) data_start;
	unsigned int rows = vertical_sections+1;
	unsigned int columns = horizontal_sections+1;

	for (unsigned int row_index = 0; row_index < vertical_sections; ++row_index)
	{
		unsigned int A, B, row_end;
		
		if (row_index % 2 == 0)
			//Note: Even row_index
		{
			A = row_index * columns;
			B = (row_index+1) * columns;
			row_end = A+columns;

			while (A < row_end)
			{
				//Add A and B to dest.
				dest[0] = A;
				dest[1] = B;
				dest += 2;
				//Increment (A, B)
				A += 1;
				B += 1;
			}
			//Add Degenerate Index for this row only if not last one.
			if (row_index < vertical_sections-1)
			{
				unsigned int degenerate_index = B-1;
				dest[0] = degenerate_index;
				dest += 1;
			}
		}
		else
			//Note: Odd row_index
		{
			A = horizontal_sections+row_index*columns;
			B =	horizontal_sections+(row_index+1)*columns;
			row_end = A-columns;

			while (A > row_end)
			{
				//Add A and B to dest.
				dest[0] = A;
				dest[1] = B;
				dest += 2;
				//Decrement (A, B)
				A -= 1;
				B -= 1;
			}
			//Add Degenerate Index for this row only if not last one.
			if (row_index < vertical_sections-1)
			{
				unsigned int degenerate_index = B+1;
				dest[0] = degenerate_index;
				dest += 1;
			}
		}
	}
}

//*****************************************************************************
//Note: Triangle Strip Algorithm for Sectioned Quad.
//	-> Vertices are numbered like this in Vertex Buffer:
//		Example for (H, V) = (4, 2):
//			14-------13-------12-------11-------10	
//          |        |        |        |        |
//          9--------8--------7--------6--------5
//          |        |        |        |        |
//          4--------3--------2--------1--------0
//
//	-> Strip like so:
//		-> First row:
//			-> 0, 5,, 1, 6,, 2, 7,, 3, 8,, 4, 9,
//		-> Then add Degenerate Indices:
//			-> 9, 5
//		-> Continue with second row:
//			-> 5, 10,, 6, 11,, 7, 12,, 8, 13,, 9, 14
//	-> Notice the indices come in pairs:
//		-> First row: (0, 5), (1, 6) ...
//		-> Second Row: (5, 10), (6, 11), ...
//	-> Insights:
//		-> Define (C, R) = (H+1, V+1)
//		-> Start pair in a row is (A, B) = (row_index * C, (row_index+1) * C)
//			-> In example: 
//				-> H = 4, C = H+1 = 5
//				-> start pair in row 0 = (0 * 5, 1 * 5) = (0, 5)
//				-> start pair in row 1 = (1 * 5, 2 * 5) = (5, 10)
//		-> To get next pair in a row:
//			-> Increment.
//				-> (c, d) = (a+1, b+1)
//					-> where (c, d) is next pair and (a, b) is previous pair.
//		-> End a row when current.A = start.A+C
//			-> Then add degenerates indicies:
//				-> current.B-1, current.A
//				-> In example:
//					-> Stop Row 0 when we hit pair (5, 10)
//						-> (5, 10) because A == 5 == start.A+C == 0+5.
//					-> Append degenerate index: 10-1 = 9
//					-> Append degenerate index: 5
//
//*****************************************************************************

void ProceduralQuad::GetTerrainStripIndices(void * data_start)
{
	unsigned int * dest = (unsigned int *) data_start;
	unsigned int rows = vertical_sections+1;
	unsigned int columns = horizontal_sections+1;

	for (unsigned int row_index = 0; row_index < vertical_sections; ++row_index)
	{
		unsigned int A, B, row_end;
		
		A = row_index * columns;
		B = (row_index+1) * columns;
		row_end = A+columns;

		while (A < row_end)
		{
			//Add A and B to dest.
			dest[0] = A;
			dest[1] = B;
			dest += 2;
			//Increment (A, B)
			A += 1;
			B += 1;
		}
		//Add Degenerate Indices for this row only if not last one.
		if (row_index < vertical_sections-1)
		{
			unsigned int degenerate_index0 = B-1;
			unsigned int degenerate_index1 = A;
			dest[0] = degenerate_index0;
			dest[1] = degenerate_index1;
			dest += 2;
		}
	}
}

unsigned int ProceduralQuad::GetTerrainStripIndexCount()
{
	//Note: 
	//	-> For each vertical_section: 2 * C 
	//		-> (not counting degenerate indices)
	//	-> # of degenerate indices = (V - 1) * 2
	unsigned int A = ((horizontal_sections+1)*2) * vertical_sections;
	unsigned int B = (vertical_sections - 1);
	return ((horizontal_sections+1)*2) * vertical_sections + (vertical_sections - 1)*2;
}


unsigned int ProceduralQuad::GetTriangleListIndexCount()
{
	return 0;
}
unsigned int ProceduralQuad::GetTriangleStripIndexCount()
{
	//Note: 
	//	-> For each vertical_section: 2 * C 
	//		-> (not counting degenerate indices)
	//	-> # of degenerate indices = V - 1
	unsigned int A = ((horizontal_sections+1)*2) * vertical_sections;
	unsigned int B = (vertical_sections - 1);
	return ((horizontal_sections+1)*2) * vertical_sections + (vertical_sections - 1);
}
