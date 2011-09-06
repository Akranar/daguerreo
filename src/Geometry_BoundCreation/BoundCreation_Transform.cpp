#include "BoundCreation.h"
#include "ExLib_BoundingVolume.h"
#include "ExLib_Eigen.h"

void BoundCreation::TransformAABB(const Eigen::Affine3f & transform, const AABB * aabb, AABB * rtn_aabb)
{
	BoundCreation::AABBFromRotatedAABB(aabb, transform, rtn_aabb);
}
void BoundCreation::TransformOBB(const Eigen::Affine3f & transform, const OBB * obb, OBB * rtn_obb)
{
	//Fix later.
	/*
	rtn_obb->SetCenter(transform * obb->GetCenter());
	Eigen::Vector3f axes[3];
	axes[0] = transform.linear() * obb->GetAxis(0);
	axes[1] = transform.linear() * obb->GetAxis(1);
	axes[2] = transform.linear() * obb->GetAxis(2);
	rtn_obb->SetExtent(0, rtn_obb->GetExtent(0) * axes[0].norm());
	rtn_obb->SetExtent(1, rtn_obb->GetExtent(1) * axes[1].norm());
	rtn_obb->SetExtent(2, rtn_obb->GetExtent(2) * axes[2].norm());
	axes[0].normalize();
	axes[1].normalize();
	axes[2].normalize();

	rtn_obb->SetAxis(0, axes[0]);
	rtn_obb->SetAxis(1, axes[1]);
	rtn_obb->SetAxis(2, axes[2]);
	*/

	//or?
	//uniform scaling only:
	
	float norm = transform.linear().norm();
	rtn_obb->SetCenter(transform * obb->GetCenter());
	Eigen::Vector3f axes[3];
	axes[0] = transform.linear() * obb->GetAxis(0);
	axes[1] = transform.linear() * obb->GetAxis(1);
	axes[2] = transform.linear() * obb->GetAxis(2);
	rtn_obb->SetExtent(0, rtn_obb->GetExtent(0) * norm);
	rtn_obb->SetExtent(1, rtn_obb->GetExtent(1) * norm);
	rtn_obb->SetExtent(2, rtn_obb->GetExtent(2) * norm);
	axes[0].normalize();
	axes[1].normalize();
	axes[2].normalize();

	rtn_obb->SetAxis(0, axes[0]);
	rtn_obb->SetAxis(1, axes[1]);
	rtn_obb->SetAxis(2, axes[2]);
	

}
void BoundCreation::TransformSphere(const Eigen::Affine3f & transform, const BoundingSphere * sphere, BoundingSphere * rtn_sphere)
{
	//Scales on X-axis only.
	//rtn_sphere->SetRadius((transform * Eigen::Vector3f(sphere->GetRadius(), 0, 0)).norm());
	float abs_max_row_sum = std::max(transform.linear().row(0).cwiseAbs().sum(), std::max(transform.linear().row(1).cwiseAbs().sum(), transform.linear().row(2).cwiseAbs().sum()));
	rtn_sphere->SetRadius(abs_max_row_sum * sphere->GetRadius());
	rtn_sphere->SetCenter(transform * sphere->GetCenter());
}

void BoundCreation::TransformBound(const Eigen::Affine3f & transform, const BoundingVolume * in, BoundingVolume * out)
{
	switch (in->GetType())
	{
		case BoundingVolume::BV_AABB:
			TransformAABB(transform, static_cast<const AABB*>(in), static_cast<AABB*>(out));
			break;
		case BoundingVolume::BV_SPHERE:
			TransformSphere(transform, static_cast<const BoundingSphere*>(in), static_cast<BoundingSphere*>(out));
			break;
		case BoundingVolume::BV_OBB:
			TransformOBB(transform, static_cast<const OBB*>(in), static_cast<OBB*>(out));
			break;
		default:
			throw 1;
			//Type not recognized.
	};
}