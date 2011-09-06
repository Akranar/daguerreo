#include "BoundCreation.h"
#include "ExLib_BoundingVolume.h"
#include "ExLib_Eigen.h"

void BoundCreation::MergeAABBs(const AABB * aabb_a, const AABB * aabb_b, AABB * rtn_aabb)
{
	Eigen::Vector3f a_min(aabb_a->GetCenter() - aabb_a->GetExtents());
	Eigen::Vector3f a_max(aabb_a->GetCenter() + aabb_a->GetExtents());
	Eigen::Vector3f b_min(aabb_b->GetCenter() - aabb_b->GetExtents());
	Eigen::Vector3f b_max(aabb_b->GetCenter() + aabb_b->GetExtents());
	Eigen::Vector3f rtn_min;
	Eigen::Vector3f rtn_max;

	rtn_min[0] = std::min(a_min[0], b_min[0]);
	rtn_min[1] = std::min(a_min[1], b_min[1]);
	rtn_min[2] = std::min(a_min[2], b_min[2]);

	rtn_max[0] = std::max(a_max[0], b_max[0]);
	rtn_max[1] = std::max(a_max[1], b_max[1]);
	rtn_max[2] = std::max(a_max[2], b_max[2]);

	rtn_aabb->SetExtents((rtn_max - rtn_min) * 0.5f);
	rtn_aabb->SetCenter((rtn_max + rtn_min) * 0.5f);
}
void BoundCreation::MergeOBBs(const OBB * obb_a, const OBB * obb_b, OBB * rtn_obb)
{

}
void BoundCreation::MergeSpheres(const BoundingSphere * sphere_a, const BoundingSphere * sphere_b, BoundingSphere * rtn_sphere)
{

}

void BoundCreation::MergeBound(const BoundingVolume * bound_a, const BoundingVolume * bound_b, BoundingVolume * rtn_bound)
{
	switch (bound_a->GetType())
	{
		case BoundingVolume::BV_AABB:
			MergeAABBs(static_cast<const AABB*>(bound_a), static_cast<const AABB*>(bound_b), static_cast<AABB*>(rtn_bound));
			break;
		case BoundingVolume::BV_SPHERE:
			MergeSpheres(static_cast<const BoundingSphere*>(bound_a), static_cast<const BoundingSphere*>(bound_b), static_cast<BoundingSphere*>(rtn_bound));
			break;
		case BoundingVolume::BV_OBB:
			MergeOBBs(static_cast<const OBB*>(bound_a), static_cast<const OBB*>(bound_b), static_cast<OBB*>(rtn_bound));
			break;
		default:
			throw 1;
			//Type not recognized.
	};
}