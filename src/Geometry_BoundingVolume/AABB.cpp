#include "AABB.h"
#include "OBB.h"
#include "BoundingSphere.h"
#include "AABB.h"

#include "ExLib_Eigen.h"
#include "ExLib_FloatDataAccessor.h"

BoundingVolume::BVType AABB::GetType() const
{
	return BoundingVolume::BV_AABB;
}

void AABB::GetVertices(FloatDataAccessor & accessor) const
{
	Eigen::Vector3f vertices[8];
	vertices[0] = GetCenter();
	vertices[0][0] += GetExtent(0);
	vertices[0][1] += GetExtent(1);
	vertices[0][2] += GetExtent(2);

	vertices[1] = GetCenter();
	vertices[1][0] -= GetExtent(0);
	vertices[1][1] += GetExtent(1);
	vertices[1][2] += GetExtent(2);

	vertices[2] = GetCenter();
	vertices[2][0] -= GetExtent(0);
	vertices[2][1] -= GetExtent(1);
	vertices[2][2] += GetExtent(2);

	vertices[3] = GetCenter();
	vertices[3][0] += GetExtent(0);
	vertices[3][1] -= GetExtent(1);
	vertices[3][2] += GetExtent(2);

	vertices[4] = GetCenter();
	vertices[4][0] += GetExtent(0);
	vertices[4][1] += GetExtent(1);
	vertices[4][2] -= GetExtent(2);

	vertices[5] = GetCenter();
	vertices[5][0] += GetExtent(0);
	vertices[5][1] -= GetExtent(1);
	vertices[5][2] -= GetExtent(2);

	vertices[6] = GetCenter();
	vertices[6][0] -= GetExtent(0);
	vertices[6][1] -= GetExtent(1);
	vertices[6][2] -= GetExtent(2);

	vertices[7] = GetCenter();
	vertices[7][0] -= GetExtent(0);
	vertices[7][1] += GetExtent(1);
	vertices[7][2] -= GetExtent(2);

	VEC3(accessor[0]) = vertices[0];
	VEC3(accessor[1]) = vertices[1];
	VEC3(accessor[2]) = vertices[2];
	VEC3(accessor[3]) = vertices[3];
	VEC3(accessor[4]) = vertices[4];
	VEC3(accessor[5]) = vertices[5];
	VEC3(accessor[6]) = vertices[6];
	VEC3(accessor[7]) = vertices[7];
}

void AABB::GetIBufferLines(unsigned char * ibuffer) const
{
	//TOP CEILING
	ibuffer[0] = 0;
	ibuffer[1] = 1;
	ibuffer[2] = 1;
	ibuffer[3] = 2;
	ibuffer[4] = 2;
	ibuffer[5] = 3;
	ibuffer[6] = 3;
	ibuffer[7] = 0;

	//BOTTOM FLOOR
	ibuffer[8] = 4;
	ibuffer[9] = 5;
	ibuffer[10] = 5;
	ibuffer[11] = 6;
	ibuffer[12] = 6;
	ibuffer[13] = 7;
	ibuffer[14] = 7;
	ibuffer[15] = 4;

	//PILLARS
	ibuffer[16] = 0;
	ibuffer[17] = 4;
	ibuffer[18] = 1;
	ibuffer[19] = 7;
	ibuffer[20] = 2;
	ibuffer[21] = 6;
	ibuffer[22] = 3;
	ibuffer[23] = 5;
}

unsigned int AABB::GetIBufferLinesCount() const
{
	return 24;
}


void AABB::GetIBufferTriangles(unsigned char * rtn_ibuffer) const
{
	//TOP
	rtn_ibuffer[0] = 0;
	rtn_ibuffer[1] = 1;
	rtn_ibuffer[2] = 3;

	rtn_ibuffer[3] = 3;
	rtn_ibuffer[4] = 1;
	rtn_ibuffer[5] = 2;

	//BOTTOM
	rtn_ibuffer[6] = 7;
	rtn_ibuffer[7] = 5;
	rtn_ibuffer[8] = 6;

	rtn_ibuffer[9] = 7;
	rtn_ibuffer[10] = 4;
	rtn_ibuffer[11] = 5;

	//LEFT
	rtn_ibuffer[12] = 1;
	rtn_ibuffer[13] = 6;
	rtn_ibuffer[14] = 2;

	rtn_ibuffer[15] = 1;
	rtn_ibuffer[16] = 7;
	rtn_ibuffer[17] = 6;

	//FRONT
	rtn_ibuffer[18] = 3;
	rtn_ibuffer[19] = 2;
	rtn_ibuffer[20] = 6;

	rtn_ibuffer[21] = 3;
	rtn_ibuffer[22] = 6;
	rtn_ibuffer[23] = 5;

	//RIGHT
	rtn_ibuffer[24] = 0;
	rtn_ibuffer[25] = 3;
	rtn_ibuffer[26] = 5;

	rtn_ibuffer[27] = 0;
	rtn_ibuffer[28] = 5;
	rtn_ibuffer[29] = 4;

	//BACK
	rtn_ibuffer[30] = 1;
	rtn_ibuffer[31] = 4;
	rtn_ibuffer[32] = 7;

	rtn_ibuffer[33] = 1;
	rtn_ibuffer[34] = 0;
	rtn_ibuffer[35] = 4;
}

unsigned int AABB::GetIBufferTrianglesCount() const
{
	return 36;
}


//Serialization
void AABB::Serialize(Stream & stream) const
{
	stream.Save(center[0]);
	stream.Save(center[1]);
	stream.Save(center[2]);

	stream.Save(extents[0]);
	stream.Save(extents[1]);
	stream.Save(extents[2]);
}
void AABB::Unserialize(Stream & stream)
{
	stream.Load(center[0]);
	stream.Load(center[1]);
	stream.Load(center[2]);

	stream.Load(extents[0]);
	stream.Load(extents[1]);
	stream.Load(extents[2]);
}
void AABB::SerializeTypeId(Stream & stream) const
{
	stream.Save("AABB");
}