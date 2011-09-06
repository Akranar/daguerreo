#include "BoundCreation.h"
#include "ExLib_BoundingVolume.h"

void BoundCreation::CopyAABB(const AABB * aabb, AABB * rtn_aabb)
{
	rtn_aabb->SetCenter(aabb->GetCenter());
	rtn_aabb->SetExtents(aabb->GetExtents());
}
void BoundCreation::CopyOBB(const OBB * obb, OBB * rtn_obb)
{
	rtn_obb->SetCenter(obb->GetCenter());
	rtn_obb->SetExtent(obb->GetExtents());
	rtn_obb->SetAxis(0, obb->GetAxis(0));
	rtn_obb->SetAxis(1, obb->GetAxis(1));
	rtn_obb->SetAxis(2, obb->GetAxis(2));
}
void BoundCreation::CopySphere(const BoundingSphere * sphere, BoundingSphere * rtn_sphere)
{
	rtn_sphere->SetCenter(sphere->GetCenter());
	rtn_sphere->SetRadius(sphere->GetRadius());
}
void BoundCreation::CopyBound(const BoundingVolume * bound, BoundingVolume * rtn_bound)
{
	switch (bound->GetType())
	{
		case BoundingVolume::BV_AABB:
			CopyAABB(static_cast<const AABB *>(bound), static_cast<AABB *> (rtn_bound));
			break;
		case BoundingVolume::BV_SPHERE:
			CopySphere(static_cast<const BoundingSphere *>(bound), static_cast<BoundingSphere *> (rtn_bound));
			break;
		case BoundingVolume::BV_OBB:
			CopyOBB(static_cast<const OBB *>(bound), static_cast<OBB *> (rtn_bound));
			break;
		default:
			throw 1;
			//Type not recognized.
	};
}