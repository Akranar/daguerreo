#ifndef BoundCreation_H_
#define BoundCreation_H_

#include "ExLib_Eigen.h"

class BoundingVolume;
class BoundingSphere;
class AABB;
class OBB;

class Transformation;

class Cone;
class Cylinder;
class Line;
class LineSegment;
class Plane;
class Polygon;
class Polyhedron;
class Ray;
class Rectangle3D;
class Tetrahedron;
class Triangle;
class Quadrilateral;

class FloatDataAccessor;

class BoundCreation
{
private:
	static void ExtremePointsAlongDirection(const Eigen::Vector3f & dir, const FloatDataAccessor & point_set, unsigned int & min_index, unsigned int & max_index);
	static void ExtremePointsAlongStandardAxes(const FloatDataAccessor & point_set, 
												   unsigned int & x_min_index, unsigned int & x_max_index,
												   unsigned int & y_min_index, unsigned int & y_max_index,
												   unsigned int & z_min_index, unsigned int & z_max_index
												   );
	static void CovarianceMatrix(const FloatDataAccessor & point_set, Eigen::Matrix3f & rtn_covariance);
public:
	static void AABBFromSphere(const BoundingSphere * sphere, AABB * rtn_aabb);
	static void AABBFromPointSet(const FloatDataAccessor & point_set, AABB * rtn_aabb);
	static void AABBFromRotatedAABB(const AABB * aabb, const Eigen::Affine3f & transformation, AABB * rtn_aabb);
	static void SphereFromAABB(const AABB * aabb, BoundingSphere * rtn_sphere);
	static void SphereFromPointSet(const FloatDataAccessor & point_set, BoundingSphere * rtn_sphere);
	static void SphereFromMaximumSpread(const FloatDataAccessor & point_set, BoundingSphere * rtn_sphere);
	static void SphereFromIterativeRefinement(const FloatDataAccessor & point_set, BoundingSphere * rtn_sphere);
	static void SphereFromWelzl(BoundingSphere * rtn_sphere);
	static void OBBFromMinimumAABB(OBB * rtn_obb);
	static void OBBFromConvexHull(const FloatDataAccessor & point_set, OBB * rtn_obb);
	static void OBBFromOptimization(const OBB * obb, OBB * rtn_obb);
	static void OBBFromBruteForce(OBB * rtn_obb);

	static AABB * NewAABB(const FloatDataAccessor & point_set);
	static OBB * NewOBB(const FloatDataAccessor & point_set);
	static BoundingSphere * NewSphere(const FloatDataAccessor & point_set);
	static BoundingVolume * NewOfSameType(const BoundingVolume * bound);

	static void MergeAABBs(const AABB * aabb_a, const AABB * aabb_b, AABB * rtn_aabb);
	static void MergeOBBs(const OBB * obb_a, const OBB * obb_b, OBB * rtn_obb);
	static void MergeSpheres(const BoundingSphere * sphere_a, const BoundingSphere * sphere_b, BoundingSphere * rtn_sphere);
	static void MergeBound(const BoundingVolume * bound_a, const BoundingVolume * bound_b, BoundingVolume * rtn_bound);

	static void CopyAABB(const AABB * aabb, AABB * rtn_aabb);
	static void CopyOBB(const OBB * obb, OBB * rtn_obb);
	static void CopySphere(const BoundingSphere * sphere, BoundingSphere * rtn_sphere);
	static void CopyBound(const BoundingVolume * bound, BoundingVolume * rtn_bound);

	static void TransformAABB(const Eigen::Affine3f & transform, const AABB * aabb, AABB * rtn_aabb);
	static void TransformOBB(const Eigen::Affine3f & transform, const OBB * obb, OBB * rtn_obb);
	static void TransformSphere(const Eigen::Affine3f & transform, const BoundingSphere * sphere, BoundingSphere * rtn_sphere);
	static void TransformBound(const Eigen::Affine3f & transform, const BoundingVolume * in, BoundingVolume * out);

	
};

#endif