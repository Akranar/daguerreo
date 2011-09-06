#include "BoundCreation.h"
#include "ExLib_BoundingVolume.h"

AABB * BoundCreation::NewAABB(const FloatDataAccessor & point_set)
{
	AABB * rtn = new AABB();
	BoundCreation::AABBFromPointSet(point_set, rtn);
	return rtn;
}
OBB * BoundCreation::NewOBB(const FloatDataAccessor & point_set)
{
	OBB * rtn = new OBB();
	return rtn;
}
BoundingSphere * BoundCreation::NewSphere(const FloatDataAccessor & point_set)
{
	BoundingSphere * rtn = new BoundingSphere();
	BoundCreation::SphereFromPointSet(point_set, rtn);
	return rtn;
}
BoundingVolume * BoundCreation::NewOfSameType(const BoundingVolume * bound)
{
	switch (bound->GetType())
	{
		case BoundingVolume::BV_AABB:
			return new AABB();
			break;
		case BoundingVolume::BV_SPHERE:
			return new BoundingSphere();
			break;
		case BoundingVolume::BV_OBB:
			return new OBB();
			break;
		default:
			throw 1;
			//Type not recognized.
	};
}