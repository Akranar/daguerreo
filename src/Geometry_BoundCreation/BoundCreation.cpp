#include "BoundCreation.h"
#include "ExLib_BoundingVolume.h"
#include <Core_RandGen.h>
#include "ExLib_FloatDataAccessor.h"
#include "ExLib_QuickHull.h"
#include "ExLib_Topology.h"

void BoundCreation::AABBFromSphere(const BoundingSphere * sphere, AABB * rtn_aabb)
{
	/*
		Main Idea:
            -> Make bounding AABB large enough to contain object at any orientation.

        -> No need to rotate AABB, just translate.
            -> But might as well use bounding sphere.
	*/
	rtn_aabb->SetCenter(sphere->GetCenter());
	rtn_aabb->SetExtent(0, sphere->GetRadius());
	rtn_aabb->SetExtent(1, sphere->GetRadius());
	rtn_aabb->SetExtent(2, sphere->GetRadius());
}

void BoundCreation::AABBFromPointSet(const FloatDataAccessor & point_set, AABB * rtn_aabb)
{
	/*
		Main Idea:
            -> Find the extreme vertices in all six directions of the coordinate axes.

        Straight forward approach:
            -> Loop through all vertices to find extreme vertices.
                -> Projecting them onto the direction vector.
            
            Note:
                -> Linear time in vertex count.
                    -> Preprocess so that all "k" vertices on convex hull come first in vertex array.
                        -> Win for concave objects but no improvement for convex.

        Use Dobkin-Kirkpatrick hierarchy (Chapter 9)
            -> Overkill in most circumstances.
                -> Extra memory and overhead in traversing hierarchy.
	*/
	unsigned int index_xMax, index_xMin, index_yMax, index_yMin, index_zMax, index_zMin;
	ExtremePointsAlongDirection(Eigen::Vector3f(1, 0, 0), point_set, index_xMin, index_xMax);
	ExtremePointsAlongDirection(Eigen::Vector3f(0, 1, 0), point_set, index_yMin, index_yMax);
	ExtremePointsAlongDirection(Eigen::Vector3f(0, 0, 1), point_set, index_zMin, index_zMax);

	Eigen::Vector3f center;
	center[0] = (VEC3(point_set[index_xMin])[0] + VEC3(point_set[index_xMax])[0]) * 0.5f;
	center[1] = (VEC3(point_set[index_yMin])[1] + VEC3(point_set[index_yMax])[1]) * 0.5f;
	center[2] = (VEC3(point_set[index_zMin])[2] + VEC3(point_set[index_zMax])[2]) * 0.5f;

	rtn_aabb->SetCenter(center);
	rtn_aabb->SetExtent(0, VEC3(point_set[index_xMax])[0] - center[0]);
	rtn_aabb->SetExtent(1, VEC3(point_set[index_yMax])[1] - center[1]);
	rtn_aabb->SetExtent(2, VEC3(point_set[index_zMax])[2] - center[2]);
}
void BoundCreation::AABBFromRotatedAABB(const AABB * aabb, const Eigen::Affine3f & transformation, AABB * rtn_aabb)
{
	/*
		Main Idea:
            -> Wrap rotated AABB in a new AABB.

        Caution:
            -> Avoid repeatedly wrapping or AABBs will grow out of control.

        Details:
            Let:
                -> A = original AABB in local space.
                    -> Using min-max representation.
                -> M = column rotation matrix.
                -> A' = M * A
                -> B = new AABB bounding A' in world space.

            -> Insight: For "x" extents of B, only the "x" components of column vectors of of M contribute. (First row)

                -> To find "x" extents of B, find the vertices that produce the minimal and maximal products with the first row of M.
                    -> For each of A's min_x and max_x values, multiply them by their corresponding entry of the first row of M.
                        -> Clarification: Since "x" is first, multiply them by the first entry in the first row of M.

                    -> Then figure out which of the two products are larger and add them to a total.

                    -> Repeat with min_y and max_y values.

                    -> Repeat with min_z and max_z values.

                -> Similar reasoning for "y" and "z" extents using 2nd and 3rd row of M respectively.

            -> Translations can be simply added in.

        Notes:
            -> Can store OBBs but intersect them as AABBs.
                Costs:
                    -> More memory to store OBB.
                    -> Extra matrix-matrix multiplcations for transforming with M.
                Benefits:
                    -> Tighter AABBs by starting with OBB.
	*/
	rtn_aabb->SetCenter(transformation * aabb->GetCenter());
	rtn_aabb->SetExtents(transformation.linear().matrix().cwiseAbs() * aabb->GetExtents());


}
void BoundCreation::SphereFromAABB(const AABB * aabb, BoundingSphere * rtn_sphere)
{
	/*
		Weak but Fast Method:
            -> Compute AABB of all points.
            -> Set Center as midpoint of AABB.
            -> Set Radius as farthest point from Center.

            Caution:
                -> Do NOT use geometric center (the mean) of all points.
                    -> Will get bad bounding spheres for nonuniformly distributed points.
                        -> Up to twice the needed radius.
	*/
	rtn_sphere->SetCenter(aabb->GetCenter());
	rtn_sphere->SetRadius(std::max(std::max(aabb->GetExtent(0), aabb->GetExtent(1)), aabb->GetExtent(2)));
}
void BoundCreation::SphereFromPointSet(const FloatDataAccessor & point_set, BoundingSphere * rtn_sphere)
{
	/*	
		Alternative Ritter Approach:
            First Pass: - Obtaining approximination.

                -> Compute 6 extremal points along axes.
                -> Choose from the 6, 2 points farthest apart.
                -> Set Center as midpoint of the two chosen points.
                -> Set Radius as half of the distance betweent he two chosen points.

            Second Pass: - Updating the sphere.

                -> Loop through all vertices outside of sphere.
                    -> Update with new sphere just large enough to emcompass old sphere and new point.
                        -> New Radius = (old_radius + distance_from_point) * 0.5
                        -> New Center += unit_direction_to_point * (new_radius - old_radius)
	*/

	/*
		First Pass {
	*/
	unsigned int index_xMin, index_xMax, index_yMin, index_yMax, index_zMin, index_zMax;
	ExtremePointsAlongStandardAxes(point_set, index_xMin, index_xMax, index_yMin, index_yMax, index_zMin, index_zMax);

	float dist2x = (VEC3(point_set[index_xMax]) - VEC3(point_set[index_xMin])).squaredNorm();
	float dist2y = (VEC3(point_set[index_yMax]) - VEC3(point_set[index_yMin])).squaredNorm();
	float dist2z = (VEC3(point_set[index_zMax]) - VEC3(point_set[index_zMin])).squaredNorm();

	int min = index_xMin, max = index_xMax;
	if (dist2y > dist2x && dist2y > dist2z)
	{
		max = index_yMax;
		min = index_yMin;
	}
	if (dist2z > dist2x && dist2z > dist2y)
	{
		max = index_zMax;
		min = index_zMin;
	}

	
	rtn_sphere->SetCenter((VEC3(point_set[min]) + VEC3(point_set[max])) * 0.5f);
	rtn_sphere->SetRadius((VEC3(point_set[max]) - rtn_sphere->GetCenter()).norm());
	/*
		First Pass }
		Second Pass {
	*/
	for (unsigned int i = 0; i < point_set.GetCount(); ++i)
	{
		Eigen::Vector3f d = VEC3(point_set[min]) - rtn_sphere->GetCenter();
		float distance_squared = d.squaredNorm();
		if (distance_squared > rtn_sphere->GetRadius() * rtn_sphere->GetRadius())
		{
			float dist = sqrt(distance_squared);
			float new_radius = (rtn_sphere->GetRadius() + dist) * 0.5f;
			float k = (new_radius - rtn_sphere->GetRadius()) / dist;
			rtn_sphere->SetRadius(new_radius);
			rtn_sphere->SetCenter(rtn_sphere->GetCenter() + d * k);
		}
	}

	/*
		Second Pass }
	*/

}
void BoundCreation::SphereFromMaximumSpread(const FloatDataAccessor & point_set, BoundingSphere * rtn_sphere)
{
	/*
		Main Idea:
            -> Use statistics to find the direction of maximum spread.

        Concepts:
            Mean:
                -> u = Average.

            Variance:
                -> variance = 1/n * sum(  i, 1, n, exp( x_i - u, 2)  )
                -> variance = 1/n * sum(  i, 1, n, exp( x_i, 2 )  ) - u*u
                    -> Intuition: average squared deviation from the mean.
                -> sqrt(variance) = standard deviation.

            Covariance:
                -> Of two variables.
                    -> Their tendency to vary together.
                    -> Conventionally computed as Covariance Matrix.
                        -> Symmetric.
                            -> Can use Jacobi Method to compute eigenvalues and eigenvectors.
                                -> The largest eigenvalue's eigenvector is the axis along which the point data has the largest variance.
	*/

}
void BoundCreation::SphereFromIterativeRefinement(const FloatDataAccessor & point_set, BoundingSphere * rtn_sphere)
{
	/*
		Main Idea:
            -> Use Ritter Method (Section 4.3.2) to make sphere.
            -> Shrink the sphere so it is no longer bound, then make the sphere bound again using the second pass of Ritter's Method.
                -> Randomize the points that are considered to improve results.
	*/
	const int NUM_ITER = 8;
	SphereFromPointSet(point_set, rtn_sphere);
	BoundingSphere s2 = *rtn_sphere;
	for (unsigned int k = 0; k < NUM_ITER; ++k)
	{
		s2.SetRadius(s2.GetRadius() * 0.95f);

		for (unsigned int i = 0; i < point_set.GetCount(); ++i)
		{
			//DoRandomSwap();
			
		}
		if (s2.GetRadius() < rtn_sphere->GetRadius()) *rtn_sphere = s2;
	}

}
void BoundCreation::SphereFromWelzl(BoundingSphere * rtn_sphere)
{
	/*
		Main Idea:
            -> Welzl's Algorithm based on:
                -> Insight: If have a minimum bounding sphere for a point set P.
                    -> When we add a point to P,
                        -> Only need to recompute sphere if point is outside of sphere.
        
        -> Minimum bounding sphere is:
            -> defined by four points from the set.
            -> bounds all points.
            -> smallest radius.

        -> Can brute force by picking four points, then testing if sphere bounds all points and has smallest radius.
        
        -> Welzl's algorithm:
            -> Maintain:
                -> Set of input points
                -> Set of support
                    -> The points from input set that must be on boundary of minimum sphere.

            Procedure:
                -> Base case:
                    -> Input set empty.
                        -> Compute and return the sphere using the support set. (zero to four points)

                -> Pick random point in input set.

                -> Recursively compute smallest sphere of remaining points.
                    -> If the selected point is inside this sphere, return the sphere.

                -> Update support set to contain selected point.

                -> Recursively compute smallest sphere of remaining points with new support set.
	*/
}

void OBBFromMinimumAABB(OBB * rtn_obb)
{
	/*
	Simple Algorithm:
        -> Compute minimum AABB of the set.

        -> Select a pair of points on the two parallel sides that are farthest apart on the AABB.
            -> Determine the length direction of the OBB.

        -> Project the set of points onto the plane perpendicular to the OBB length direction.

        -> Compute the minimum axis-aligned rectangle on this plane.

        -> Select the pair of points ont he two parallel sides to determine the 2nd axis of the OBB.

        -> The third axis is the cross product of the 1st and 2nd axes.
	*/
}

void BoundCreation::OBBFromConvexHull(const FloatDataAccessor & point_set, OBB * rtn_obb)
{
	/*
		Covariance Methods:
            -> Should avoid methods based on weighting vertex positions.
                -> Nonuniform distribution can bias covariance.
                -> Defining features of Minimum bounding volume independent of vertex clustering.
                -> Need more than vertices to produce reliable covariance matrices.

            -> Use a continuous formulation of covariance
                -> Calculate Covariance Matrix using convex hull.
                    -> Let:
                        -> (Pk, Qk, Rk) = triangles in the convex hull
                            -> where 0 <= k < n

                    -> C_ij = 1/Ah * sum (k, 0, n, (Ak / 12) * (9 * Mki * Mkj + Pki * Pkj + Qki * Qkj + Rki * Rkj) ) - Mhi * Mhj
                        -> where Ak = area of the triangle. (cross product of edges divided by 2)

                        -> where Mk = (Pk + Qk + Rk) / 3 = centroid of triangle k.

                        -> where Ah = sum(k, 0, n, Ak)
                            -> Total area of convex hull

                        -> where Mh = 1/Ah * sum(k, 0, n, Ah * Mk)
                            -> mean of the triangle centroids weighted by their area.

                        -> where i, j = subscripts indicate which coordinate component is taken (x, y, z)

            -> With Covariance Matrix, use Jacobi method to extract eigenvalues and eigenvectors as in Section 4.3.3
	*/

	WingedTriangleMesh convex_hull;
	QuickHull::ConvexHull(point_set, convex_hull);

	// To finish
	

}
void BoundCreation::OBBFromOptimization(const OBB * obb, OBB * rtn_obb)
{
	/*
		Main Idea:
            -> Have a starting OBB.

            One Interation:
                -> Pick a direction of the OBB
                -> Project all vertices onto the plane perpendicular to that direction.
                -> Update the OBB to align with the minimum area bounding rectangle of the projection.

        Building Minimum Rectangle:
            Insight:
                -> The minimum bounding rectangle of a convex polygon has at least
                    -> one side collinear with an edge of the polygon.

            Method 1:
                -> Compute convex hull of point set
                -> For each edge:
                    -> Choose it as one side of the rectangle.
                    -> Also get its perpendicular.
                    -> Project all points onto these two axes and find the area.
                -> Find the edge that corresponds to the minimum rectangle area.

            Method 2 - Rotating Calipers:
                -> Compute convex polygon of points
                -> Create a rectangle bounding the polygon
                    -> by drawing four lines through extreme points of the polygon.
                    -> choose at least one side coincident with an edge of the polygon.
                -> Rotate all lines until a line coincides with an edge of the polygon.
                -> Repeat rotation until a total 90 degrees have been rotated.
                -> Pick the rectangle corresponding to the least area during the rotation.
	*/

}
void BoundCreation::OBBFromBruteForce(OBB * rtn_obb)
{
	/*
		Main Idea:
            -> Parameterize the space of OBB orientations.
            -> Sample the space, choose the interval with the best OBB.
            -> Refine the OBB orientation by sampling the best interval at a higher resolution.
                -> Hill-climbing approach.
                -> Terminate when can't find any better orientation.
	*/
}