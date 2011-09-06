#include "OBB.h"

BoundingVolume::BVType OBB::GetType() const
{
	return BoundingVolume::BV_OBB;
}

void OBB::GetRotationMatrix(Eigen::Matrix3f & rtn_matrix) const
{
	rtn_matrix(0) = GetAxis(0)[0];
	rtn_matrix(1) = GetAxis(1)[0];
	rtn_matrix(2) = GetAxis(2)[0];

	rtn_matrix(3) = GetAxis(0)[1];
	rtn_matrix(4) = GetAxis(1)[1];
	rtn_matrix(5) = GetAxis(2)[1];

	rtn_matrix(6) = GetAxis(0)[2];
	rtn_matrix(7) = GetAxis(1)[2];
	rtn_matrix(8) = GetAxis(2)[2];
}


//Serialization
void OBB::Serialize(Stream & stream) const
{
	stream.Save(center[0]);
	stream.Save(center[1]);
	stream.Save(center[2]);

	stream.Save(axes[0][0]);
	stream.Save(axes[0][1]);
	stream.Save(axes[0][2]);

	stream.Save(axes[1][0]);
	stream.Save(axes[1][1]);
	stream.Save(axes[1][2]);

	stream.Save(axes[2][0]);
	stream.Save(axes[2][1]);
	stream.Save(axes[2][2]);

	stream.Save(extents[0]);
	stream.Save(extents[1]);
	stream.Save(extents[2]);
}
void OBB::Unserialize(Stream & stream)
{
	stream.Load(center[0]);
	stream.Load(center[1]);
	stream.Load(center[2]);

	stream.Load(axes[0][0]);
	stream.Load(axes[0][1]);
	stream.Load(axes[0][2]);

	stream.Load(axes[1][0]);
	stream.Load(axes[1][1]);
	stream.Load(axes[1][2]);

	stream.Load(axes[2][0]);
	stream.Load(axes[2][1]);
	stream.Load(axes[2][2]);

	stream.Load(extents[0]);
	stream.Load(extents[1]);
	stream.Load(extents[2]);
}
void OBB::SerializeTypeId(Stream & stream) const
{
	stream.Save("OBB");
}