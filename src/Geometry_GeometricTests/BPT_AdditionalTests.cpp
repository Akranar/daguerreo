#include "BasicPrimitiveTests.h"

#include "GeometricPrimitives.h"

bool BasicPrimitiveTests::TestingPointInPolygon(const Eigen::Vector3f & point, const Polygon & polygon)
{
	/*
		Main Idea:
            Binary Search Method:
                -> Uses binary search to find where in the polygon the point is in.
            Counting ray enter/exits:
                -> Shoots a ray from the point along an axis and counts how many times it enters and exits the polygon.
	*/
	return false;
}
bool BasicPrimitiveTests::TestingPointInTriangle(const Eigen::Vector3f & point, const Triangle & triangle)
{
	/*
		Main Idea:
            -> Translates both triangle and the point so the point is at the origin. Then test if the origin is contained in the triangle.

        General Idea:
            -> P lies inside ABC IFF
                -> Triangles PAB, PBC, PCA are all same winding (clockwise or counter-clockwise).

        Test:
            -> Since P is now origin:
                -> "General Idea" can be tested with:
                    -> dot(u, v) >= 0 and
                    -> dot(u, w) >= 0
                        -> where u = cross(B, C)
                        -> where v = cross(C, A)
                        -> where w = cross(A, B)
	*/
	return false;
}
bool BasicPrimitiveTests::TestingPointInPolyhedron(const Eigen::Vector3f & point, const Polyhedron & polyhedron)
{
	/*
		Main Idea:
            Method 1:
                -> Test if the point is in each of the halfspaces defining the polyhedron.
            Method 2:
                -> Build BSP tree and see if point ends up in a solid leaf (Chapter 8)
                    -> Polyhedron must be given as closed mesh.
            Method 3:
                -> GJK Method (Chapter 9)
            Method 4:
                -> Shoot a ray from point in any direction and count the number of faces intersected.
	*/
	return false;
}
bool BasicPrimitiveTests::IntersectionOfTwoPlanes(const Plane & plane_a, const Plane & plane_b)
{
	/*
		Main Idea:
            -> Construct the line that the planes intersect at.

        -> Let:
            -> Plane 1: dot(n1, X) = d1
            -> Plane 2: dot(n2, X) = d2
        -> L = P + t * d
            -> d = cross(n1, n2)

            -> Getting P:
                -> Since P is spanned by both n1 and n2
                    -> P = k1 * n1 + k2 * n2
                -> P must lie on both plane 1 and plane 2
                    -> Must satisfy both equations.

                -> Get system of equations:
                    -> dot(n1, k1 * n1 + k2 * n2) = d_1
                    -> dot(n2, k1 * n1 + k2 * n2) = d_2

                -> Solution:
                    -> k1 = (d1 * dot(n2, n2) - d2 * dot(n1, n2))    /    dot(n1, n1) * dot(n2, n2) - exp(  dot(n1, n2)  ,  2  )
                    -> k2 = (d2 * dot(n1, n1) - d1 * dot(n1, n2))    /    dot(n1, n1) * dot(n2, n2) - exp(  dot(n1, n2)  ,  2  )

        Optimization:
            -> Use Lagrange's Identity
                -> denom = dot(n1, n1) * dot(n2, n2) - exp(   dot(n1, n2)  ,  2  )
                    -> = dot(cross(n1, n2), cross(n1, n2))
                        -> = dot(d, d)

            -> Use vector identity: cross(u, cross(v, w)) = dot(u, w) * v - dot(v, w) * u
                -> Simplify P = k1 * n1 + k2 * n2
                    -> By substituting k1 and k2 + shitload of algebra
                
                    -> P = cross(d1 * n2 - d2 * n1, d)    /    dot(d, d)
                        -> where d = cross(n1, n2)
	*/
	return false;
}
bool BasicPrimitiveTests::IntersectionOfThreePlanes(const Plane & plane_a, const Plane & plane_b, const Plane & plane_c)
{
	/*
		Five Cases:
            (0) All three planes parallel/coplanar to one another.
            (1) All three intersect at single line.
            (2) Two planes parallel; Last plane intersects both.
                -> Forms two parallel intersection lines.
            (3) Planes pairwise intersect.
                -> Forms three parallel intersection lines.
            (4) All planes intersect at single point.

        Let:
            -> Planes 1-3:
                -> dot(ni, X) = di
                    -> where i = 1, 2, 3

        Solving:
            -> Case 4:
                -> Identified by:
                    -> Scalar Triple Product: [n1, n2, n3] = 0

            -> Other cases identified similarly.

            -> Intersect at point X = (x1, x2, x3)
                -> Solve as 3x3 system of linear equations using plane equations with Cramer's Rule.
                    -> x1 = det( d, m2, m3 )    /    denom
                    -> x2 = det( m1, d, m3 )    /    denom
                    -> x3 = det( m1, m2, d )    /    denom
                        -> where m1 = (n1.x, n2.x, n3.x)
                        -> where m2 = (n1.y, n2.y, n3.f)
                        -> where m3 = (n1.z, n2.z, n3.z)
                        -> where d = (d1, d2, d3)
                        -> where denom = det( m1, m2, m3 )

                -> X simplifies to:
                    -> x1 = dot(d, u)    /    denom
                    -> x2 = dot(m3, v)    /    denom
                    -> x3 = dot(-m2, v)    /    denom
                        -> where u = cross(m2, m3)
                        -> where v = cross(m1, d)
                        -> where denom = dot(m1, u)

        Alternative Approach:
            -> Use X = (    d1 * cross(n2, n3) + d2 * cross(n3, n1) + d3 * cross(n1, n2)    /    dot(n1, cross(n2, n3))    )

                -> Derived by noticing that X = linear combination of the directions of the lines of intersection of the planes.
                    -> with scalars a, b, c
                        -> Insert these as point in each of three plane equations, simplifying and solving for "a", "b", "c"
                            -> Then plugging a, b, c back into original expression of X as linear combo
                                -> And simplifying.
	*/
	return false;
}


bool BasicPrimitiveTests::IntersectLineAgainstLine(const Line & line_a, const Line & line_b, float & rtn_s, float & rtn_t)
{
	ClosestPointOfTwoLines(line_a, line_b, rtn_s, rtn_t);
	Eigen::Vector3f point_a, point_b;
	line_a.GetPointAt(rtn_s, point_a);
	line_b.GetPointAt(rtn_t, point_b);
	return ((point_a - point_b).squaredNorm() < 1e-12);
}

bool BasicPrimitiveTests::IntersectLineAgainstSegment(const Line & line_a, const LineSegment & segment, float & rtn_s, float & rtn_t)
{
	float rtn_value = IntersectLineAgainstLine(line_a, Line(segment.GetPointA(), segment.GetPointB() - segment.GetPointA()), rtn_s, rtn_t);
	return rtn_value && Between0And1(rtn_t);
}