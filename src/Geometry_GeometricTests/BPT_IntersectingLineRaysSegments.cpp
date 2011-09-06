#include "BasicPrimitiveTests.h"
#include "GeometricPrimitives.h"
#include "ExLib_BoundingVolume.h"

#include <algorithm>
#include <float.h>

#define EPSILON 1e-6

bool BasicPrimitiveTests::IntersectingSegmentAgainstPlane(const LineSegment & segment, const Plane & plane, float & rtn_t)
{
	/*
		Main Idea:
            -> Subsitutes line equation into plane to find intersection
                -> Tests if intersection is within segment endpoints

        Let:
            -> Line Segment S:
                -> S(t) = A + t * (B-A)
                    -> for 0 <= t <= 1
            -> Plane P:
                -> dot(n, X) = d

        Solve for "t" of intersection:
            -> t = (    d - dot(n, A)    /    dot(n, B-A)    )

        -> Return intersection only if:
            -> 0 <= t <= 1
	*/
	
	rtn_t = (    plane.GetD() - plane.GetNormal().dot(segment.GetPointA())    )    /    plane.GetNormal().dot(segment.GetPointB() - segment.GetPointA());
	return (rtn_t <= 1.0f && rtn_t >= 0.0f);
	
}
bool BasicPrimitiveTests::IntersectingRayAgainstSphere(const Ray & ray, const BoundingSphere & sphere, float & rtn_t)
{
	/*
		Main idea:
            - Ray is substituted into sphere equation. Then solve quadratic formula for intersection.
                - Test if intersection is within segment/ray endpoints

        -> Use dot(X-C, X-C) = exp(r, 2)
            -> As sphere equation.

        Solving for "t":
            -> Quadratic equation in "t" encountered.
                    -> where b = dot(m, d)
                    -> where c = dot(m, m) - r*r
						-> where m = P-C
                -> t = -b + sqrt(exp(b, 2) - c)
                -> t = -b - sqrt(exp(b, 2) - c)

        Notes:
            -> Number of real roots => number of intersections:
                -> Categorized by discriminant d = exp(b, 2) - c

            -> May have false intersection with t < 0 when ray starts from inside sphere.
	*/
	
	Eigen::Vector3f m = ray.GetOrigin() - sphere.GetCenter();
	float b = (m).dot(ray.GetDirection());
	float c = m.dot(m);
	if (c > 0.0f && b > 0.0f)
	{
		//Case: Ray origin outside of sphere and points away. => No Intersections.
		return false;
	}
	float discriminant = b * b - c;
	if (discriminant < 0.0f)
	{
		//Case: Misses sphere
		return false;
	}
	else
	{
		//Case: Hits sphere. Calculate smallest t.
		rtn_t = -b - sqrt(discriminant);
		if (rtn_t < 0.0f)
		{
			rtn_t = 0.0f;
		}
		return true;
	}
	
}

bool BasicPrimitiveTests::IntersectingLineAgainstTriangle(const Line & line, const Triangle & triangle, Eigen::Vector3f & rtn_point)
{
	/*
		Main Idea:
            -> Tests if line's intersection with triangle plane is inside triangle 
                -> Using scalar triple products to compute signed tetrahedral volumes
                    -> Which are then used to compute barycentric coordinates of the line-triangle plane intersection.

        Let:
            -> Triangle ABC
            -> Line through PQ
            -> Intersection point R
                -> between line and triangle's plane

        To only determine intersection:
            -> R must be inside triangle
                -> R must be to left of AB, BC, CA if ABC is counter-clockwise vertex ordering.
            -> Use scalar triple products to determine winding of AB, BC, CA compared to PQ:
                -> u, v, w >= 0
                    -> u = [PQ PC PB]
                    -> v = [PQ PA PC]
                    -> w = [PQ PB PA]

        To obtain intersection point:

            -> Volumes of tetrahedra RBCP, RCAP, RABP, proportional to areas of base triangles RBC, RCA, RAB because they all have same height.

            -> Barycentric points:
                -> u = (     [PQ PC PB]    /    [PQ PC PB] + [PQ PA PC] + [PQ PB PA]    )
                -> v = (     [PQ PA PC]    /    [PQ PC PB] + [PQ PA PC] + [PQ PB PA]    )
                -> w = (     [PQ PB PA]    /    [PQ PC PB] + [PQ PA PC] + [PQ PB PA]    )

	*/

	
	Eigen::Vector3f PQ = -line.GetDirection();
	Eigen::Vector3f PA = triangle.GetVertex(0) - line.GetPoint();
	Eigen::Vector3f PB = triangle.GetVertex(1) - line.GetPoint();
	Eigen::Vector3f PC = triangle.GetVertex(2) - line.GetPoint();

	Eigen::Vector3f barycentric_coords;
	barycentric_coords[0] = ScalarTripleProduct(PQ, PC, PB);
	if (barycentric_coords[0] < 0.0f) return false;
	barycentric_coords[1] = ScalarTripleProduct(PQ, PA, PC);
	if (barycentric_coords[1] < 0.0f) return false;
	barycentric_coords[2] = ScalarTripleProduct(PQ, PB, PA);
	if (barycentric_coords[2] < 0.0f) return false;

	float sum = barycentric_coords[0] + barycentric_coords[1] + barycentric_coords[2];
	if (sum < EPSILON)
	{
		float s, s0, s1, s2, t;

		LineSegment edge_AB = LineSegment(triangle.GetVertex(0), triangle.GetVertex(1));
		if (!IntersectLineAgainstSegment(line, edge_AB, s0, t)) s0 = FLT_MAX;
		LineSegment edge_BC = LineSegment(triangle.GetVertex(1), triangle.GetVertex(2));
		if (!IntersectLineAgainstSegment(line, edge_AB, s1, t)) s1 = FLT_MAX;
		LineSegment edge_AC = LineSegment(triangle.GetVertex(0), triangle.GetVertex(2));
		if (!IntersectLineAgainstSegment(line, edge_AB, s2, t)) s2 = FLT_MAX;
		s = std::min(std::min(s0, s1), s2);
		line.GetPointAt(s, rtn_point);
		return true;
	}

	float denom = 1.0f / sum;
	barycentric_coords *= denom;

	rtn_point = triangle.GetVertex(0) * barycentric_coords[0] + triangle.GetVertex(1) * barycentric_coords[1] + triangle.GetVertex(2) * barycentric_coords[2];

	return true;
	
}
bool BasicPrimitiveTests::IntersectingLineAgainstQuadrilateral(const Line & line, const Quadrilateral & quadrilateral, Eigen::Vector3f & rtn_point)
{
	/*
		Main Idea:
            -> Very similar to Section 5.3.4
            -> For Quadrilateral ABCD, test which side of the diagonal AC R would have to be on.
                -> Then correspondingly do 5.3.4 on Triangle ABC or DAC

        Notes:
            -> Assumes ABCD convex.
                -> Works for concave as long as the interior diagonal is chosen as AC.
            -> Test line PQ with AC using scalar triple product winding test. (As in Section 5.3.4)
            -> Cannot return barycentric coordinates directly but can return actual point.
	*/
	return false;
}

int BasicPrimitiveTests::IntersectingSegmentAgainstSphere(const LineSegment & segment, const BoundingSphere & sphere, float & rtn_t1, float & rtn_t2)
{
	
	Eigen::Vector3f m = segment.GetPointA() - sphere.GetCenter();
	float b = (m).dot(segment.GetPointB() - segment.GetPointA());
	float c = m.dot(m);
	float discriminant = b * b - c;

	int intersection_count = 0;
	float t1 = -b - sqrt(discriminant);
	float t2 = -b + sqrt(discriminant);
	if (t1 >= 0.0f && t1 <= 1.0f && t2 >= 0.0f && t2 <= 1.0f)
	{
		rtn_t1 = t1;
		rtn_t2 = t2;	
		return 2;
	}
	else if (t1 >= 0.0f && t1 <= 1.0f)
	{
		rtn_t1 = t1;
		return 1;
	}
	else if (t2 >= 0.0f && t2 <= 1.0f)
	{
		rtn_t1 = t2;
		return 1;
	}
	
	return 0;
}


bool BasicPrimitiveTests::IntersectingRayAgainstAABB(const Ray & ray, const AABB & aabb, float & rtn_t)
{
	/*
		Main Idea:
            -> Uses slab representation for box.
            -> Finds intersection of ray/segment with each slab. Then compares intersection times for overlap in all slabs.
                -> Keep track of:
                    -> A: The farthest of all entries into a slab
                    -> B: The closest of all exits out of a slab.
                -> If A > B at anytime, exit with no intersection. 

        Intersecting slabs:
            -> Intersect slabs by inserting ray equation into plane equations for slab.
            -> Solve for t

        -> Must handle case when ray parallel to slab separately.
            -> To avoid division by zero.

        Can test for intersection without calculating intersection point:
            -> Choose coordinate system where box is axis aligned and centered at origin:
                AABB:
                    -> Translate segment and AABB to origin.
                OBB:
                    -> Transform Segment to OBB space, then translate both segment and OBB to origin.

            -> Do separating axis test with 6 axes:
                -> Three principle axes.
                -> Three cross products of box face normals and segment direction vector.
	*/
	
	rtn_t = 0.0f;
	float tmax = FLT_MAX;
	Eigen::Vector3f aabb_min = aabb.GetCenter() - aabb.GetExtents();
	Eigen::Vector3f aabb_max = aabb.GetCenter() + aabb.GetExtents();
	for (int i = 0; i < 3; ++i)
	{
		if (abs(ray.GetDirection()[i]) < EPSILON)
		{
			//Ray is parallel to slab. Not hit if origin not within slab.
			if (ray.GetOrigin()[i] < aabb_min[i] || ray.GetOrigin()[i] > aabb_max[i])
			{
				return false;
			}
		}
		else
		{
			float one_over_direction = 1.0f / ray.GetDirection()[i];
			float t1 = (aabb_min[i] - ray.GetOrigin()[i]) * one_over_direction;
			float t2 = (aabb_max[i] - ray.GetOrigin()[i]) * one_over_direction;
			if (t1 > t2) Swap(t1, t2);
			if (t1 > rtn_t) rtn_t = t1;
			if (t2 > tmax) tmax = t2;
			if (rtn_t > tmax) return false;
		}
	}
	
	return true;
}

bool BasicPrimitiveTests::IntersectingRayAgainstOBB(const Ray & ray, const OBB & obb, float & rtn_t)
{
	
	Eigen::Matrix3f obb_rotation;
	obb.GetRotationMatrix(obb_rotation);
	Eigen::Vector3f ray_d_obb = obb_rotation * ray.GetDirection();
	Eigen::Vector3f ray_o_obb = obb_rotation * (ray.GetOrigin() - obb.GetCenter());

	
	rtn_t = 0.0f;
	float tmax = FLT_MAX;
	Eigen::Vector3f aabb_min = -obb.GetExtents();
	Eigen::Vector3f aabb_max = obb.GetExtents();
	for (int i = 0; i < 3; ++i)
	{
		if (abs(ray_d_obb[i]) < EPSILON)
		{
			//Ray is parallel to slab. Not hit if origin not within slab.
			if (ray_o_obb[i] < aabb_min[i] || ray_o_obb[i] > aabb_max[i])
			{
				return false;
			}
		}
		else
		{
			float one_over_direction = 1.0f / ray_d_obb[i];
			float t1 = (aabb_min[i] - ray_o_obb[i]) * one_over_direction;
			float t2 = (aabb_max[i] - ray_o_obb[i]) * one_over_direction;
			if (t1 > t2) Swap(t1, t2);
			if (t1 > rtn_t) rtn_t = t1;
			if (t2 > tmax) tmax = t2;
			if (rtn_t > tmax) return false;
		}
	}
	
	
	return true;
}

int BasicPrimitiveTests::IntersectingSegmentAgainstAABB(const LineSegment & segment, const AABB & aabb, float & rtn_t1, float & rtn_t2)
{
	
	rtn_t1 = 0.0f;
	rtn_t2 = FLT_MAX;
	Eigen::Vector3f aabb_min = aabb.GetCenter() - aabb.GetExtents();
	Eigen::Vector3f aabb_max = aabb.GetCenter() + aabb.GetExtents();
	Eigen::Vector3f segment_direction = segment.GetPointB() - segment.GetPointA();
	for (int i = 0; i < 3; ++i)
	{
		if (abs(segment_direction[i]) < EPSILON)
		{
			//Ray is parallel to slab. Not hit if origin not within slab.
			if (segment.GetPointA()[i] < aabb_min[i] || segment.GetPointA()[i] > aabb_max[i])
			{
				return 0;
			}
		}
		else
		{
			float one_over_direction = 1.0f / segment_direction[i];
			float t1 = (aabb_min[i] - segment.GetPointA()[i]) * one_over_direction;
			float t2 = (aabb_max[i] - segment.GetPointA()[i]) * one_over_direction;
			if (t1 > 1.0f && t2 > 1.0f)
			{
				return 0;
			}
			if (t1 > t2) Swap(t1, t2);
			if (t1 > rtn_t1) rtn_t1 = t1;
			if (t2 > rtn_t2) rtn_t2 = t2;
			if (rtn_t1 > rtn_t2) return 0;
		}
	}
	if (rtn_t1 >= 0.0f && rtn_t1 <= 1.0f)
	{
		if (rtn_t2 >= 0.0f && rtn_t2 <= 1.0f)
		{
			return 2;
		}
		else
		{
			return 1;
		}
	}
	else
	{
		Swap(rtn_t1, rtn_t2);
		if (rtn_t1 >= 0.0f && rtn_t1 <= 1.0f)
		{
			return 1;
		}
		return 0;
	}
	
}

int BasicPrimitiveTests::IntersectingSegmentAgainstOBB(const LineSegment & segment, const OBB & obb, float & rtn_t1, float & rtn_t2)
{
	
	Eigen::Matrix3f obb_rotation;
	obb.GetRotationMatrix(obb_rotation);
	Eigen::Vector3f segment_A_obb = obb_rotation * (segment.GetPointA() - obb.GetCenter());
	Eigen::Vector3f segment_B_obb = obb_rotation * (segment.GetPointB() - obb.GetCenter());
	
	rtn_t1 = 0.0f;
	rtn_t2 = FLT_MAX;
	Eigen::Vector3f aabb_min = -obb.GetExtents();
	Eigen::Vector3f aabb_max = obb.GetExtents();
	Eigen::Vector3f segment_direction = segment_B_obb - segment_A_obb;
	for (int i = 0; i < 3; ++i)
	{
		if (abs(segment_direction[i]) < EPSILON)
		{
			//Ray is parallel to slab. Not hit if origin not within slab.
			if (segment_A_obb[i] < aabb_min[i] || segment_A_obb[i] > aabb_max[i])
			{
				return 0;
			}
		}
		else
		{
			float one_over_direction = 1.0f / segment_direction[i];
			float t1 = (aabb_min[i] - segment_A_obb[i]) * one_over_direction;
			float t2 = (aabb_max[i] - segment_A_obb[i]) * one_over_direction;
			if (t1 > 1.0f && t2 > 1.0f)
			{
				return 0;
			}
			if (t1 > t2) Swap(t1, t2);
			if (t1 > rtn_t1) rtn_t1 = t1;
			if (t2 > rtn_t2) rtn_t2 = t2;
			if (rtn_t1 > rtn_t2) return 0;
		}
	}
	if (rtn_t1 >= 0.0f && rtn_t1 <= 1.0f)
	{
		if (rtn_t2 >= 0.0f && rtn_t2 <= 1.0f)
		{
			return 2;
		}
		else
		{
			return 1;
		}
	}
	else
	{
		Swap(rtn_t1, rtn_t2);
		if (rtn_t1 >= 0.0f && rtn_t1 <= 1.0f)
		{
			return 1;
		}
		return 0;
	}
	
}

bool BasicPrimitiveTests::IntersectingRayAgainstAABB(const Ray & ray, const AABB & aabb)
{
	
	const Eigen::Vector3f & ray_d_aabb = ray.GetDirection();
	Eigen::Vector3f ray_o_aabb = ray.GetOrigin() - aabb.GetCenter();
	
	return 0;
}
bool BasicPrimitiveTests::IntersectingRayAgainstOBB(const Ray & ray, const OBB & obb)
{
	
	Eigen::Matrix3f obb_rotation;
	obb.GetRotationMatrix(obb_rotation);
	Eigen::Vector3f ray_d_obb = obb_rotation * ray.GetDirection();
	Eigen::Vector3f ray_o_obb = obb_rotation * (ray.GetOrigin() - obb.GetCenter());
	
	return 0;
}
int BasicPrimitiveTests::IntersectingSegmentAgainstAABB(const LineSegment & segment, const AABB & aabb)
{
	
	Eigen::Vector3f segment_a_aabb = segment.GetPointA() - aabb.GetCenter();
	Eigen::Vector3f segment_b_aabb = segment.GetPointB() - aabb.GetCenter();
	Eigen::Vector3f segment_m_aabb = (segment_a_aabb + segment_b_aabb) * 0.5f;
	
	return 0;
}
int BasicPrimitiveTests::IntersectingSegmentAgainstOBB(const LineSegment & segment, const OBB & obb)
{
	
	Eigen::Matrix3f obb_rotation;
	obb.GetRotationMatrix(obb_rotation);
	Eigen::Vector3f segment_A_obb = obb_rotation * (segment.GetPointA() - obb.GetCenter());
	Eigen::Vector3f segment_B_obb = obb_rotation * (segment.GetPointB() - obb.GetCenter());
	
	return 0;
}

bool BasicPrimitiveTests::IntersectingRayAgainstTriangle(const Ray & ray, const Triangle & triangle, Eigen::Vector3f & rtn_barycentric_coords)
{
	/*
		Main Idea:
            -> Represent triangle in barycentric parametric form.
            -> Represent line parametric explicitly. 
            -> Set triangle equation equal to line equation and solve for parameters, t, v, and w.
                -> Verify parameters are within bounds.

        Let:
            -> Triangle:
                -> T(u, v, w) = A + v * (B-A) + w * (C-A)
                    -> with u = 1-v-w
                    -> where v >= 0
                    -> where w >= 0
                    -> where v + w <= 1
            -> Line:
                -> R(t) = P + t * (Q-P)

        Solve for t, v, w:
            -> Get 3x3 System of Linear Equations
            -> Solve with Cramer's Rule
                    -> where n = cross(B-A, C-A)
                    -> where d = dot(P-Q, n)
                    -> where e = cross(P-Q, P-A)
                -> t = dot(P-A, n)    /    d
                -> v = dot(C-A, e)    /    d
                -> w = -dot(B-A, e)    /    d
	*/
	return 0;
}
bool BasicPrimitiveTests::IntersectingRayAgainstCylinder(const Ray & ray, const Cylinder & cylinder, float & rtn_t)
{
	/*
		Let:
            -> Cylinder:
                -> Two points P and Q
                -> Radius r

            -> Line:
                -> L(t) = A + t * (B-A)

        Point X is on surface of Cylinder when:
            -> dot(v-w, v-w) - r*r = 0
                -> where v = X-P

                -> where d = Q-P
                    -> cylinder's axis.

                -> w = (    dot(v, d)    /    dot(d, d)    ) * d
                    -> Component of "v" parallel to "d"
            
            Intuition:
                -> v-w's length is equal to "r"
                    -> v-w is perpendicular to cylinder's axis "d"

        Procedure for Infinite Cylinder:
            -> Substitute Line into Cylinder as X
            -> Get quadratic expression.
                -> Number of real roots => number of intersections.
                -> t = (    - b (plus/minus) sqrt(b*b - a*c)    /    a    )
                    -> where: rewritten with Lagrange's Identity:
                        -> where a = dot(cross(d, n), cross(d, n))
                        -> where b = dot(cross(d, m), cross(d, n))
                        -> where c = dot(cross(d, m), cross(d, m)) - r * r * dot(d, d)
                            -> where m = A-P
                            -> where n = B-A

        Procedure for Finite Cylinder:

            -> Must check if intersection point at "t" is outside endcap planes through P and Q.
                -> L(t) lies outside plane through P when
                    -> dot(L(t)-P, d) < 0
                        -> if dot(n, d) <= 0,
                            -> L points away from or parallel to P endcap
                        -> if dot(n, d) > 0,
                            -> Intersection at endcap:
                                -> Plugin L(t) as X in equation for endcap plane: dot(X-P, d) = 0
                                    -> t = (    - dot(m, d)    /     dot(n, d)    )
                                        -> Must check if in circle:
                                            -> dot(L(t) - P, L(t) - P) <= r*r
                -> L(t) lies outside plane through Q when
                    -> dot(L(t)-P, d) > dot(d, d)
                        -> if (n, d) < 0
                            -> Intersection at endcap:
                                -> Plug in L(t) as X in equation for endcap plane: dot(X-Q, d) = 0
                                    -> t = (    dot(d, d) - dot(m, d)    /    dot(n, d)    )
                                        -> Then check if within endcap circle.
	*/
	return 0;
}
bool BasicPrimitiveTests::IntersectingRayAgainstConvexPolyhedron(const Ray & ray, const Polyhedron & polyhedron, float & rtn_t)
{
	/*
		Main Idea:
            -> Intersect Ray/Segment with each negative halfspace defined by each face of the polyhedron,
                -> continuously clipping the intersection interval of the ray/segment until a small interval remains or no interval remains.

        Let:
            -> Segment: S(t) = A + t * (B-A)
            -> Plane: dot(n, X) = d
            -> t_first = largest "t" for segment entering a plane's halfspace.
                -> Init: 0
            -> t_last = smallest "t" for segment exiting a plane's halfspace.
                -> Init: 1

        Test:
            -> Intersection with plane given at:

                -> t = (    d - dot(n, A)    /    dot(n, B-A)    )
                    -> Plane faces segment when denominator dot(n, B-A) < 0

                        -> Means "t" is when segment enters halfspace
                            -> clip t_first to "t" if:
                                -> t > t_first

                    -> Plane faces away from segment when denominator dot(n, B-A) > 0

                        -> Means "t" is when segment exits halfspace
                            -> clip t_last to "t" if:
                                -> t < t_last

            -> If ever t_last < t_first
                -> No Intersection
	*/
	return 0;
}
bool BasicPrimitiveTests::IntersectingSegmentAgainstTriangle(const LineSegment & segment, const Triangle & triangle, Eigen::Vector3f & rtn_barycentric_coords)
{
	return 0;
}
int BasicPrimitiveTests::IntersectingSegmentAgainstCylinder(const LineSegment & segment, const Cylinder & cylinder, float & rtn_t1, float & rtn_t2)
{
	return 0;
}
int BasicPrimitiveTests::IntersectingSegmentAgainstConvexPolyhedron(const LineSegment & segment, const Polyhedron & polyhedron, float & rtn_t1, float & rtn_t2)
{
	return 0;
}